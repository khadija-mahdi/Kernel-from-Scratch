#ifndef PANIC_H
#define PANIC_H

#include <lib/lib.h>

/**
 * Kernel Panic
 * Prints an error message, dumps CPU registers, and halts the system.
 */

void kernel_panic(const char *message);

#endif /* PANIC_H */
