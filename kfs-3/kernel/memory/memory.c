#include <kernel/memory/memory.h>
#include <kernel/memory/pmm.h>
#include <kernel/memory/vmm.h>
#include <kernel/memory/vmalloc.h>

void initMemory(uint32_t memHigh, uint32_t physicalAllocStart)
{
    mem_num_vpages = 0;

    /* Remove identity mapping for first 4MB */
    initial_page_dir[0] = 0;
    invalidate(0);

    /* Set up recursive page directory entry */
    initial_page_dir[1023] = ((uint32_t)initial_page_dir - KERNEL_START)
                             | PAGE_FLAG_PRESENT | PAGE_FLAG_WRITE;
    invalidate(0xFFFFF000);

    /* Initialize subsystems */
    pmm_init(physicalAllocStart, memHigh);
    vmmInit();
    vmallocInit();
}
