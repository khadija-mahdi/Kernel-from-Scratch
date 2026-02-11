#include <kernel/memory/vmm.h>
#include <kernel/memory/pmm.h>
#include <kernel/memory/memory.h>

#define NUM_PAGES_DIRS 256

int mem_num_vpages;

static uint32_t pageDirs[NUM_PAGES_DIRS][1024] __attribute__((aligned(4096)));
static uint8_t  pageDirUsed[NUM_PAGES_DIRS];

/* ── TLB invalidation ──────────────────────────────────────── */

void invalidate(uint32_t vaddr)
{
    asm volatile("invlpg %0" :: "m"(vaddr));
}

/* ── Page directory helpers ─────────────────────────────────── */

uint32_t *memGetCurrentPageDir(void)
{
    uint32_t pd;
    asm volatile("mov %%cr3, %0" : "=r"(pd));
    pd += KERNEL_START;
    return (uint32_t *)pd;
}

void memChangePageDir(uint32_t *pd)
{
    pd = (uint32_t *)(((uint32_t)pd) - KERNEL_START);
    asm volatile("mov %0, %%eax \n mov %%eax, %%cr3 \n" :: "m"(pd));
}

void syncPageDirs(void)
{
    for (int i = 0; i < NUM_PAGES_DIRS; i++) {
        if (pageDirUsed[i]) {
            uint32_t *pageDir = pageDirs[i];
            for (int j = 768; j < 1023; j++) {
                pageDir[j] = initial_page_dir[j] & ~PAGE_FLAG_OWNER;
            }
        }
    }
}

/* ── Init (called from memory.c) ───────────────────────────── */

void vmmInit(void)
{
    memset(pageDirs, 0, 0x1000 * NUM_PAGES_DIRS);
    memset(pageDirUsed, 0, NUM_PAGES_DIRS);
}

/* ── Map a virtual page to a physical frame ─────────────────── */

void memMapPage(uint32_t virtualAddr, uint32_t physAddr, uint32_t flags)
{
    uint32_t *prevPageDir = 0;

    if (virtualAddr >= KERNEL_START) {
        prevPageDir = memGetCurrentPageDir();
        if (prevPageDir != initial_page_dir)
            memChangePageDir(initial_page_dir);
    }

    uint32_t pdIndex = virtualAddr >> 22;
    uint32_t ptIndex = (virtualAddr >> 12) & 0x3FF;

    uint32_t *pageDir = REC_PAGEDIR;
    uint32_t *pt      = REC_PAGETABLE(pdIndex);

    if (!(pageDir[pdIndex] & PAGE_FLAG_PRESENT)) {
        uint32_t ptPAddr = pmmAllocPageFrame();
        pageDir[pdIndex] = ptPAddr | PAGE_FLAG_PRESENT | PAGE_FLAG_WRITE
                           | PAGE_FLAG_OWNER | flags;
        invalidate(virtualAddr);

        for (uint32_t i = 0; i < 1024; i++)
            pt[i] = 0;
    }

    pt[ptIndex] = physAddr | PAGE_FLAG_PRESENT | flags;
    mem_num_vpages++;
    invalidate(virtualAddr);

    if (prevPageDir != 0) {
        syncPageDirs();
        if (prevPageDir != initial_page_dir)
            memChangePageDir(prevPageDir);
    }
}

/* ── Unmap a virtual page ───────────────────────────────────── */

void memUnmapPage(uint32_t virtualAddr)
{
    uint32_t pdIndex = virtualAddr >> 22;
    uint32_t ptIndex = (virtualAddr >> 12) & 0x3FF;

    uint32_t *pageDir = REC_PAGEDIR;
    uint32_t *pt      = REC_PAGETABLE(pdIndex);

    if (pageDir[pdIndex] & PAGE_FLAG_PRESENT) {
        pt[ptIndex] = 0;
        invalidate(virtualAddr);
        mem_num_vpages--;
    }
}
