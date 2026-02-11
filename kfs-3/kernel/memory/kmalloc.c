#include <kernel/memory/kmalloc.h>
#include <kernel/memory/pmm.h>
#include <kernel/memory/vmm.h>
#include <kernel/memory/memory.h>
#include <kernel/memory/panic.h>

#define BLOCK_SIZE(hdr) ((hdr) & ~1u)
#define BLOCK_USED(hdr) ((hdr) & 1u)
#define HEADER_SIZE sizeof(uint32_t)

// Make these accessible to heap_map command
uint32_t heapStart;
uint32_t heapSize;
static bool kmallocInitialized = false;

uint32_t userHeapStart;
uint32_t userHeapSize;
// static bool umallocInitialized = false;

/* ── Init ───────────────────────────────────────────────────── */

void kmallocInit(uint32_t initialHeapSize)
{
    heapStart = KERNEL_MALLOC;
    heapSize = 0;
    kmallocInitialized = true;

    changeHeapSize(initialHeapSize);
    // umallocInit();

    /* Sentinel: size=0 marks end of block list */
    *((uint32_t *)heapStart) = 0;
}


void changeHeapSize(int newSize)
{
    int oldPageTop = CEIL_DIV(heapSize, 0x1000);
    int newPageTop = CEIL_DIV(newSize, 0x1000);

    if (newPageTop > oldPageTop)
    {
        int diff = newPageTop - oldPageTop;
        for (int i = 0; i < diff; i++)
        {
            uint32_t phys = pmmAllocPageFrame();
            memMapPage(KERNEL_MALLOC + oldPageTop * 0x1000 + i * 0x1000,
                       phys, PAGE_FLAG_WRITE);
        }
    }

    heapSize = newSize;
}

/* ── Allocate ───────────────────────────────────────────────── */

void *kmalloc(uint32_t size)
{
    // Input validation
    if (!kmallocInitialized || size == 0)
        return NULL;

    // Align size to 4 bytes
    size = (size + 3) & 0xFFFFFFFC;

    // Start at beginning of heap (VIRTUAL address 0xD0000000)
    uint32_t *currentBlock = (uint32_t *)heapStart;

    // STEP 1: Try to find a free block
    while (*currentBlock != 0)
    {
        uint32_t blockSize = BLOCK_SIZE(*currentBlock);
        bool isUsed = BLOCK_USED(*currentBlock);

        // Found a free block big enough?
        if (!isUsed && blockSize >= size)
        {
            // Can we split this block?
            if (blockSize >= size + HEADER_SIZE + 4)
            {
                // Split: mark first part as used, keep second part free
                uint32_t *nextBlock = (uint32_t *)((uint8_t *)currentBlock + HEADER_SIZE + size);
                *nextBlock = (blockSize - size - HEADER_SIZE); // Free remainder
                *currentBlock = size | 1;                      // Mark used (bit 0 = 1)
            }
            else
            {
                // Too small to split, use entire block
                *currentBlock |= 1; // Mark used
            }

            // Return VIRTUAL address (e.g., 0xD0001234)
            // MMU will translate this to physical when CPU accesses it
            return (void *)((uint8_t *)currentBlock + HEADER_SIZE);
        }

        // Move to next block
        currentBlock = (uint32_t *)((uint8_t *)currentBlock + HEADER_SIZE + blockSize);
    }

    // STEP 2: No free block found — allocate at end of heap
    uint32_t currentOffset = (uint32_t)((uint8_t *)currentBlock - (uint8_t *)heapStart);

    // Need more pages?
    if (currentOffset + size + HEADER_SIZE > (uint32_t)heapSize)
        changeHeapSize(heapSize + size + 0x100000);

    // Mark this block as used
    *currentBlock = size | 1;

    // Add sentinel (marks end of heap)
    uint32_t *sentinel = (uint32_t *)((uint8_t *)currentBlock + HEADER_SIZE + size);
    *sentinel = 0;
    return (void *)((uint8_t *)currentBlock + HEADER_SIZE);
}

/* ── Free ───────────────────────────────────────────────────── */

void kfree(void *ptr)
{
    if (!ptr)
        return;

    uint32_t *hdr = (uint32_t *)((uint8_t *)ptr - HEADER_SIZE);
    *hdr &= ~1u; /* clear allocated bit */

    /* Coalesce with next block if it is free */
    uint32_t *next = (uint32_t *)((uint8_t *)hdr + HEADER_SIZE + BLOCK_SIZE(*hdr));
    if (*next != 0 && !BLOCK_USED(*next))
        *hdr = BLOCK_SIZE(*hdr) + HEADER_SIZE + BLOCK_SIZE(*next);

    /* Coalesce with previous block (linear scan) */
    uint32_t *prev = NULL;
    uint32_t *cur = (uint32_t *)heapStart;

    while (cur < hdr && *cur != 0)
    {
        prev = cur;
        cur = (uint32_t *)((uint8_t *)cur + HEADER_SIZE + BLOCK_SIZE(*cur));
    }

    if (prev && !BLOCK_USED(*prev))
        *prev = BLOCK_SIZE(*prev) + HEADER_SIZE + BLOCK_SIZE(*hdr);
}

/* ── Query the usable size of an allocated block ────────────── */

uint32_t ksize(void *ptr)
{
    if (!ptr)
        return 0;

    uint32_t *hdr = (uint32_t *)((uint8_t *)ptr - HEADER_SIZE);
    return BLOCK_SIZE(*hdr);
}

/* ── Adjust the kernel heap break (like sbrk) ──────────────── */

void *kbrk(int32_t increment)
{
    if (increment <= 0)
        return (void *)(heapStart + heapSize);

    uint32_t oldEnd = heapStart + heapSize;
    changeHeapSize(heapSize + (uint32_t)increment);
    return (void *)oldEnd;
}
