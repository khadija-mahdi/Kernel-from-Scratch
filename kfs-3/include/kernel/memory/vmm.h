#ifndef VMM_H
#define VMM_H

#include <lib/lib.h>

/**
 * Virtual Memory Manager (VMM)
 * Manages page directories, page tables, and virtual-to-physical mappings.
 */

/* ── Page flags ─────────────────────────────────────────────── */
#define PAGE_FLAG_PRESENT   (1 << 0)
#define PAGE_FLAG_WRITE     (1 << 1)
#define PAGE_FLAG_USER      (1 << 2)
#define PAGE_FLAG_OWNER     (1 << 9)   /* custom: page table owned by us */

/* ── Recursive paging helpers ───────────────────────────────── */
#define REC_PAGEDIR         ((uint32_t *)0xFFFFF000)
#define REC_PAGETABLE(i)    ((uint32_t *)(0xFFC00000 + ((i) << 12)))

/* ── Externs from boot.s ────────────────────────────────────── */
extern uint32_t initial_page_dir[1024];
extern int      mem_num_vpages;

/* ── Functions ──────────────────────────────────────────────── */
void      vmmInit(void);
void      invalidate(uint32_t vaddr);
uint32_t *memGetCurrentPageDir(void);
void      memChangePageDir(uint32_t *pd);
void      syncPageDirs(void);
void      memMapPage(uint32_t virtualAddr, uint32_t physAddr, uint32_t flags);
void      memUnmapPage(uint32_t virtualAddr);

#endif /* VMM_H */
