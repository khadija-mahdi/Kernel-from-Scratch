#ifndef PMM_H
#define PMM_H

#include <lib/lib.h>

/**
 * Physical Memory Manager (PMM)
 * Manages physical page frames using a bitmap allocator.
 * Each bit represents one 4KB page frame.
 */

void      pmm_init(uint32_t memLow, uint32_t memHigh);
uint32_t  pmmAllocPageFrame(void);
uint32_t  pmmFreePageFrame(uint32_t addr);
uint32_t  pmmGetTotalAlloc(void);

#endif /* PMM_H */
