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

#ifndef SCENIC_ACTIVATE_H
#define SCENIC_ACTIVATE_H

#include <stdint.h>

#include "scenicos.h"

#include "arena.h"
#include "resolve.h"

int activate(
    arena               *a,
    const system_cfg    *cfg,
    const resolved_list *resolved_pkgs,
    uint32_t             generation);

int activate_rollback(arena *a);

#endif
