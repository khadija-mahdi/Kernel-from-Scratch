#include <kernel/memory/vmalloc.h>
#include <kernel/memory/vmm.h>
#include <kernel/memory/pmm.h>
#include <kernel/memory/kmalloc.h>
#include <kernel/memory/memory.h>
#include <kernel/memory/panic.h>

/* Virtual region starts after the kernel heap area */
#define VMALLOC_START 0xE0000000

typedef struct vmalloc_region {
    uint32_t                virtualAddr;
    uint32_t                numPages;
    struct vmalloc_region  *next;
} vmalloc_region_t;

static vmalloc_region_t *regionList = NULL;
static uint32_t          nextFreeVAddr = VMALLOC_START;

/* ── Init ───────────────────────────────────────────────────── */

void vmallocInit(void)
{
    regionList    = NULL;
    nextFreeVAddr = VMALLOC_START;
}

/* ── Allocate virtual pages ─────────────────────────────────── */

void *vmalloc(uint32_t size)
{
    if (size == 0)
        return NULL;

    uint32_t numPages = CEIL_DIV(size, 0x1000);
    uint32_t vaddr    = nextFreeVAddr;

    /* Map each virtual page to a fresh physical frame */
    for (uint32_t i = 0; i < numPages; i++) {
        uint32_t phys = pmmAllocPageFrame();
        if (phys == 0) {
            kernel_panic("vmalloc: out of physical memory");
            return NULL;
        }
        memMapPage(vaddr + i * 0x1000, phys, PAGE_FLAG_WRITE);
    }

    /* Record the region */
    vmalloc_region_t *region = (vmalloc_region_t *)kmalloc(sizeof(vmalloc_region_t));
    if (!region) {
        kernel_panic("vmalloc: kmalloc failed for region descriptor");
        return NULL;
    }
    region->virtualAddr = vaddr;
    region->numPages    = numPages;
    region->next        = regionList;
    regionList          = region;

    nextFreeVAddr += numPages * 0x1000;
    return (void *)vaddr;
}

/* ── Free virtual pages ─────────────────────────────────────── */

void vfree(void *ptr)
{
    if (!ptr)
        return;

    uint32_t vaddr = (uint32_t)ptr;
    vmalloc_region_t *prev = NULL;
    vmalloc_region_t *cur  = regionList;

    while (cur) {
        if (cur->virtualAddr == vaddr) {
            /* Unmap each page and free the physical frame */
            for (uint32_t i = 0; i < cur->numPages; i++) {
                uint32_t page = vaddr + i * 0x1000;
                uint32_t pdIndex = page >> 22;
                uint32_t ptIndex = (page >> 12) & 0x3FF;
                uint32_t *pt = REC_PAGETABLE(pdIndex);
                uint32_t physAddr = pt[ptIndex] & 0xFFFFF000;
                pmmFreePageFrame(physAddr);
                memUnmapPage(page);
            }
            /* Remove from linked list */
            if (prev)
                prev->next = cur->next;
            else
                regionList = cur->next;
            kfree(cur);
            return;
        }
        prev = cur;
        cur  = cur->next;
    }
}

/* ── Get size of a vmalloc'd region ─────────────────────────── */

uint32_t vsize(void *ptr)
{
    if (!ptr)
        return 0;

    uint32_t vaddr = (uint32_t)ptr;
    vmalloc_region_t *cur = regionList;

    while (cur) {
        if (cur->virtualAddr == vaddr)
            return cur->numPages * 0x1000;
        cur = cur->next;
    }
    return 0;
}

/* ── Adjust the vmalloc break (like sbrk for virtual memory) ── */

void *vbrk(int32_t increment)
{
    if (increment == 0)
        return (void *)nextFreeVAddr;

    if (increment > 0)
        return vmalloc((uint32_t)increment);

    /* Negative increment not supported — return current break */
    return (void *)nextFreeVAddr;
}
