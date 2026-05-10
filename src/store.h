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

#ifndef SCENIC_STORE_H
#define SCENIC_STORE_H

#include "scenicos.h"
#include "arena.h"
#include "resolve.h"

#define SCN_STORE_ROOT "/scn/store"

const char *store_path_compute(
    arena              *a,
    const pkg          *p,
    const resolved     *resolved_pkgs,
    size_t              n_resolved);

bool store_path_exists(const char *store_path);

int store_install(const char *temp_path, const char *store_path);

int store_remove(const char *store_path);

#endif
