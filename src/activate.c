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

#include "activate.h"

#include <errno.h>

/**
 * activate() - Build and activate a generation.
 * @a: Arena for transient allocations.
 * @cfg: System configuration to activate.
 * @resolved: Topologically-sorted resolved package list.
 * @generation: Generation number to assign.
 *
 * Symlinks resolved store paths into /scn/system/<gen>/, updates the
 * bootloader, atomically swaps /scn/system/current, and runs service
 * activations. On failure at any step, the previous generation
 * remains active.
 *
 * Return: 0 on success, errno value on failure.
 */
int activate(
    arena               *a,
    const system_cfg    *cfg,
    const resolved_list *resolved_pkgs,
    uint32_t             generation)
{
    (void)a;
    (void)cfg;
    (void)resolved_pkgs;
    (void)generation;
    return ENOSYS;
}

/**
 * activate_rollback() - Activate the previous generation.
 * @a: Arena for transient allocations.
 *
 * Return: 0 on success, errno value on failure.
 */
int activate_rollback(arena *a) {
    (void)a;
    return ENOSYS;
}
