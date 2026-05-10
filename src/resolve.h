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

#ifndef SCENIC_RESOLVE_H
#define SCENIC_RESOLVE_H

#include "arena.h"
#include "error.h"
#include "scenicos.h"

typedef struct {
    const pkg  *def;
    const char *store_path;
} resolved;

typedef struct {
    const resolved *data;
    size_t          len;
} resolved_list;

resolve_error resolve(
    arena              *a,
    const system_cfg   *cfg,
    resolved_list      *out);

#endif
