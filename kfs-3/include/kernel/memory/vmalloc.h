#ifndef VMALLOC_H
#define VMALLOC_H

#include <lib/lib.h>

/**
 * Virtual Memory Allocator
 * Allocates contiguous virtual address ranges backed by physical frames.
 */

void   vmallocInit(void);
void  *vmalloc(uint32_t size);
void   vfree(void *ptr);
uint32_t vsize(void *ptr);
void  *vbrk(int32_t increment);

#endif /* VMALLOC_H */
