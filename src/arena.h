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

#ifndef SCENIC_ARENA_H
#define SCENIC_ARENA_H

#include <stddef.h>

typedef struct arena arena;

arena *arena_create(size_t initial_capacity);
void  arena_destroy(arena *a);

void *arena_alloc(arena *a, size_t size, size_t align);
char *arena_strdup(arena *a, const char *s);
char *arena_sprintf(arena *a, const char *fmt, ...);

void arena_reset(arena *a);

size_t arena_used(const arena *a);
size_t arena_capacity(const arena *a);

#endif
