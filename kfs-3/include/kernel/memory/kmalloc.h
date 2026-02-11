#ifndef KMALLOC_H
#define KMALLOC_H

#include <lib/lib.h>

/**
 * Kernel Heap Allocator
 * Provides kmalloc/kfree/ksize/kbrk on top of the physical + virtual memory managers.
 */

void      kmallocInit(uint32_t initialHeapSize);
void      changeHeapSize(int newSize);
void     *kmalloc(uint32_t size);
void      kfree(void *ptr);
uint32_t  ksize(void *ptr);
void     *kbrk(int32_t increment);

#endif /* KMALLOC_H */