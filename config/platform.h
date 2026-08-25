#ifndef PLATFORM_H
#define PLATFORM_H

/*
 * PLATFORM_RENODE is supplied by the compiler.
 *
 * Renode build:
 *     -DPLATFORM_RENODE=1
 *
 * Hardware build:
 *     -DPLATFORM_RENODE=0
 */

#ifndef PLATFORM_RENODE
#define PLATFORM_RENODE 1
#endif

#endif /* PLATFORM_H */