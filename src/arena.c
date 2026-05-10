/* Copyright (C) 2026 tonybanters (tony@tonybtw.com)

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   The GNU General Public License is contained in the file LICENSE.
*/

#include "arena.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct block block;
struct block {
    block  *next;
    size_t  capacity;
    size_t  used;
};

struct arena {
    block  *head;
    block  *first;
    size_t  total_used;
    size_t  total_cap;
};

static block *block_create(size_t capacity) {
    block *b = malloc(sizeof(block) + capacity);
    if (b == NULL) return NULL;
    b->next = NULL;
    b->capacity = capacity;
    b->used = 0;
    return b;
}

/**
 * arena_create() - Allocate a new arena with one initial block.
 * @initial_capacity: Bytes of usable space in the first block.
 *
 * Return: Arena pointer, or NULL on allocation failure.
 */
arena *arena_create(size_t initial_capacity) {
    arena *a = malloc(sizeof(arena));
    if (a == NULL) return NULL;

    block *b = block_create(initial_capacity);
    if (b == NULL) {
        free(a);
        return NULL;
    }

    a->head = b;
    a->first = b;
    a->total_used = 0;
    a->total_cap = initial_capacity;
    return a;
}

/**
 * arena_destroy() - Free every block held by the arena.
 * @a: Arena to free. NULL is permitted.
 */
void arena_destroy(arena *a) {
    if (a == NULL) return;
    block *b = a->first;
    while (b != NULL) {
        block *next = b->next;
        free(b);
        b = next;
    }
    free(a);
}

/**
 * arena_alloc() - Bump-allocate an aligned chunk from the arena.
 * @a: Arena to allocate from.
 * @size: Number of bytes to reserve.
 * @align: Required alignment in bytes (power of two).
 *
 * A new block at twice the current capacity (or larger if the request
 * itself is larger) is allocated when the head block is exhausted.
 *
 * Return: Pointer to @size aligned bytes, or NULL on allocation failure.
 */
void *arena_alloc(arena *a, size_t size, size_t align) {
    if (a == NULL || size == 0) return NULL;

    uintptr_t base = (uintptr_t)((char *)(a->head + 1) + a->head->used);
    uintptr_t aligned = (base + (align - 1)) & ~(align - 1);
    size_t pad = aligned - base;

    if (a->head->used + pad + size <= a->head->capacity) {
        a->head->used += pad + size;
        a->total_used += pad + size;
        return (void *)aligned;
    }

    size_t new_cap = a->head->capacity * 2;
    if (new_cap < size + align) new_cap = size + align;

    block *b = block_create(new_cap);
    if (b == NULL) return NULL;

    b->next = a->head;
    a->head = b;
    a->total_cap += new_cap;

    base = (uintptr_t)(b + 1);
    aligned = (base + (align - 1)) & ~(align - 1);
    pad = aligned - base;

    b->used = pad + size;
    a->total_used += pad + size;
    return (void *)aligned;
}

/**
 * arena_strdup() - Duplicate a NUL-terminated string into the arena.
 * @a: Arena to allocate from.
 * @s: Source string. NULL yields NULL.
 *
 * Return: Owned-by-arena copy of @s, or NULL on allocation failure.
 */
char *arena_strdup(arena *a, const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s) + 1;
    char *out = arena_alloc(a, len, 1);
    if (out == NULL) return NULL;
    memcpy(out, s, len);
    return out;
}

/**
 * arena_sprintf() - printf into a fresh arena allocation.
 * @a: Arena to allocate from.
 * @fmt: printf format string.
 *
 * Return: Owned-by-arena formatted string, or NULL on allocation failure.
 */
char *arena_sprintf(arena *a, const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    int len = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);
    if (len < 0) return NULL;

    char *out = arena_alloc(a, (size_t)len + 1, 1);
    if (out == NULL) return NULL;

    va_start(ap, fmt);
    vsnprintf(out, (size_t)len + 1, fmt, ap);
    va_end(ap);
    return out;
}

/**
 * arena_reset() - Rewind the arena to empty without releasing memory.
 * @a: Arena to reset. NULL is permitted.
 *
 * Frees every block except the first; the first block's used pointer is
 * reset to zero.
 */
void arena_reset(arena *a) {
    if (a == NULL) return;

    block *b = a->head;
    while (b != a->first) {
        block *next = b->next;
        free(b);
        b = next;
    }
    a->head = a->first;
    a->first->used = 0;
    a->total_used = 0;
    a->total_cap = a->first->capacity;
}

size_t arena_used(const arena *a)     { return a == NULL ? 0 : a->total_used; }
size_t arena_capacity(const arena *a) { return a == NULL ? 0 : a->total_cap; }
