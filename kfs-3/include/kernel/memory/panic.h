#ifndef PANIC_H
#define PANIC_H

#include <lib/lib.h>

/**
 * Kernel Panic Types
 */
typedef enum {
    PANIC_FATAL,
    PANIC_OOM,
    PANIC_STACK,
    PANIC_INIT,
    PANIC_WARNING,
    PANIC_PAGE_FAULT,   
    PANIC_DOUBLE_FREE,  
    PANIC_HEAP_CORRUPT, 
    PANIC_DIVISION_ZERO,
    PANIC_GPF           
} panic_type_t;


void kpanic(panic_type_t type, const char *message);

// Individual panic handlers
void panicFatal();
void panicOutOfMemory(const char *message);
void panicStackOverflow(const char *message);
void panicInit(const char *message);
void panicWarning(const char *message);
void panicPageFault(const char *message);
void panicDoubleFree(const char *message);
void panicHeapCorrupt(const char *message);
void panicDivisionZero(const char *message);
void panicGPF(const char *message);

// Convenience macros
#define kernel_panic(msg)   kpanic(PANIC_FATAL, msg)
#define kernel_warning(msg) kpanic(PANIC_WARNING, msg)

#endif /* PANIC_H */
