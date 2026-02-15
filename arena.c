#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "./lib/arena.h"

// Align size to alignment boundary
static inline size_t align_size(size_t size, size_t alignment)
{
   return (size + alignment - 1) & ~(alignment - 1);
}

// Create a new arena block
static ArenaBlock *arena_block_create(size_t size)
{
   ArenaBlock *block = (ArenaBlock *)malloc(sizeof(ArenaBlock));
   if (!block)
      return NULL;

   block->memory = malloc(size);
   if (!block->memory)
   {
      free(block);
      return NULL;
   }

   block->size = size;
   block->used = 0;
   block->next = NULL;

   return block;
}

// Free an arena block
static void arena_block_free(ArenaBlock *block)
{
   if (!block)
      return;

   if (block->memory)
      free(block->memory);
   free(block);
}

Arena *arena_create(size_t initialSize)
{
   if (initialSize == 0)
      initialSize = ARENA_DEFAULT_SIZE;

   Arena *arena = (Arena *)malloc(sizeof(Arena));
   if (!arena)
      return NULL;

   arena->current = arena_block_create(initialSize);
   if (!arena->current)
   {
      free(arena);
      return NULL;
   }

   arena->blocks = arena->current;
   arena->totalAllocated = 0;
   arena->blockCount = 1;

   return arena;
}

void *arena_alloc_aligned(Arena *arena, size_t size, size_t alignment)
{
   if (!arena || size == 0)
      return NULL;

   // Align the size
   size_t alignedSize = align_size(size, alignment);

   // Check if current block has enough space
   ArenaBlock *block = arena->current;
   size_t availableSpace = block->size - block->used;

   if (availableSpace < alignedSize)
   {
      // Need a new block
      size_t newBlockSize = block->size * 2;
      if (newBlockSize < alignedSize)
         newBlockSize = alignedSize * 2;

      ArenaBlock *newBlock = arena_block_create(newBlockSize);
      if (!newBlock)
         return NULL;

      // Link new block
      newBlock->next = arena->blocks;
      arena->blocks = newBlock;
      arena->current = newBlock;
      arena->blockCount++;

      block = newBlock;
   }

   // Allocate from current block
   void *ptr = (char *)block->memory + block->used;
   block->used += alignedSize;
   arena->totalAllocated += alignedSize;

   // Zero the memory
   memset(ptr, 0, size);

   return ptr;
}

void *arena_alloc(Arena *arena, size_t size)
{
   return arena_alloc_aligned(arena, size, ARENA_ALIGNMENT);
}

void arena_reset(Arena *arena)
{
   if (!arena)
      return;

   // Reset all blocks
   ArenaBlock *block = arena->blocks;
   while (block)
   {
      block->used = 0;
      block = block->next;
   }

   arena->current = arena->blocks;
   arena->totalAllocated = 0;
}

void arena_free(Arena *arena)
{
   if (!arena)
      return;

   // Free all blocks
   ArenaBlock *block = arena->blocks;
   while (block)
   {
      ArenaBlock *next = block->next;
      arena_block_free(block);
      block = next;
   }

   free(arena);
}

size_t arena_total_allocated(Arena *arena)
{
   return arena ? arena->totalAllocated : 0;
}

size_t arena_block_count(Arena *arena)
{
   return arena ? arena->blockCount : 0;
}