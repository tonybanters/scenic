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

#ifndef SCENIC_BUILD_H
#define SCENIC_BUILD_H

#include "scenicos.h"

#include "arena.h"
#include "error.h"
#include "resolve.h"

realize_error build_pkg(
    arena              *a,
    const pkg          *p,
    const pkg_refs     *all_pkgs,
    const resolved     *resolved_pkgs,
    size_t              pkg_idx
);

#endif
