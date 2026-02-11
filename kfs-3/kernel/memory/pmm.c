#include <kernel/memory/pmm.h>
#include <kernel/memory/panic.h>

/* 4GB address space / 4KB pages / 8 bits per byte = bytes needed for bitmap */
#define NUM_PAGE_FRAMES (0x100000000ULL / 0x1000 / 8)

static uint32_t pageFrameMin;
static uint32_t pageFrameMax;
static uint32_t totalAlloc;

uint8_t physicalMemoryBitmap[NUM_PAGE_FRAMES / 8];

/* ── Init ───────────────────────────────────────────────────── */

void pmm_init(uint32_t memLow, uint32_t memHigh)
{
    pageFrameMin = CEIL_DIV(memLow, 0x1000);
    pageFrameMax = memHigh / 0x1000;
    totalAlloc   = 0;

    memset(physicalMemoryBitmap, 0, sizeof(physicalMemoryBitmap));
}

/* ── Allocate one 4KB page frame ────────────────────────────── */

uint32_t pmmAllocPageFrame(void)
{
    uint32_t start = pageFrameMin / 8 + ((pageFrameMin & 7) != 0 ? 1 : 0);
    uint32_t end   = pageFrameMax / 8 - ((pageFrameMax & 7) != 0 ? 1 : 0);

    for (uint32_t b = start; b < end; b++) {
        uint8_t byte = physicalMemoryBitmap[b];
        if (byte == 0xFF)
            continue;

        for (uint32_t i = 0; i < 8; i++) {
            if (!((byte >> i) & 1)) {
                physicalMemoryBitmap[b] |= (1 << i);
                totalAlloc++;
                return (b * 8 + i) * 0x1000;
            }
        }
    }

    kpanic(PANIC_OOM, "PMM: out of physical page frames");
    return 0;
}

/* ── Free one 4KB page frame ────────────────────────────────── */

uint32_t pmmFreePageFrame(uint32_t addr)
{
    if (addr % 0x1000 != 0) // 0x1000 = 4096 bytes = page size so this checks if addr is page-aligned that means it is a valid page frame address. If not, return 0 to indicate failure. 
        return 0;

    uint32_t pageFrame = addr / 0x1000;
    if (pageFrame < pageFrameMin || pageFrame >= pageFrameMax) 
        return 0;

    uint32_t b = pageFrame / 8; // byte index in bitmap 
    uint32_t i = pageFrame % 8; // remainder so this gives us the bit index within that byte.

    physicalMemoryBitmap[b] &= ~(1 << i); //in math terms, this is equivalent to physicalMemoryBitmap[b] = physicalMemoryBitmap[b] & ~(1 << i); which clears the i-th bit of the byte at index b in the bitmap.
    totalAlloc--;
    return addr;
}

/* ── Stats ──────────────────────────────────────────────────── */

uint32_t pmmGetTotalAlloc(void)
{
    return totalAlloc;
}
