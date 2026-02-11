#ifndef MEMORY_H
#define MEMORY_H

#include <lib/lib.h>

/**
 * memory.h — Shared constants and top-level init
 *
 * This header provides address-space constants used by all
 * memory subsystems (PMM, VMM, kmalloc, vmalloc).
 * Subsystem-specific declarations live in their own headers.
 */

/* ── Address space layout ───────────────────────────────────── */
#define KERNEL_START    0xC0000000
#define KERNEL_MALLOC   0xD0000000

#define USER_SPACE_START   0x00000000
#define USER_SPACE_END     0xBFFFFFFF

/* ── Top-level initializer ──────────────────────────────────── */
void initMemory(uint32_t memHigh, uint32_t physicalAllocStart);
void check_stack_overflow(void);

#endif /* MEMORY_H */