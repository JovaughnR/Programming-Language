#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>

// Arena configuration
#define ARENA_DEFAULT_SIZE (64 * 1024) // 64KB default
#define ARENA_ALIGNMENT 8              // 8-byte alignment

typedef struct ArenaBlock
{
   void *memory;
   size_t size;
   size_t used;
   struct ArenaBlock *next;
} ArenaBlock;

typedef struct Arena
{
   ArenaBlock *current;
   ArenaBlock *blocks;
   size_t totalAllocated;
   size_t blockCount;
} Arena;

// Create new arena
Arena *arena_create(size_t initialSize);

// Allocate from arena
void *arena_alloc(Arena *arena, size_t size);

// Allocate aligned memory
void *arena_alloc_aligned(Arena *arena, size_t size, size_t alignment);

// Reset arena (keep memory, reset pointer)
void arena_reset(Arena *arena);

// Free entire arena
void arena_free(Arena *arena);

// Get arena statistics
size_t arena_total_allocated(Arena *arena);
size_t arena_block_count(Arena *arena);

// Macros for type-safe allocation
#define ARENA_ALLOC(arena, type) \
   ((type *)arena_alloc(arena, sizeof(type)))

#define ARENA_ALLOC_ARRAY(arena, type, count) \
   ((type *)arena_alloc(arena, sizeof(type) * (count)))

#endif