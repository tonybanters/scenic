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

#include "realize.h"

#include "build.h"

/**
 * realize() - Build every resolved package into the store.
 * @a: Arena for transient allocations.
 * @all_pkgs: Full package list (passed through to build_pkg()).
 * @resolved: Topologically-sorted resolved list. Builds happen in order.
 *
 * Stops at the first failure and returns its error.
 *
 * Return: REALIZE_OK_VAL on success, the failing package's error otherwise.
 */
realize_error realize(
    arena              *a,
    const pkg_refs     *all_pkgs,
    const resolved_list *resolved_pkgs
) {
    for (size_t i = 0; i < resolved_pkgs->len; i++) {
        realize_error err = build_pkg(
            a,
            resolved_pkgs->data[i].def,
            all_pkgs,
            resolved_pkgs->data,
            i
        );
        if (err.kind != REALIZE_OK) return err;
    }
    return REALIZE_OK_VAL;
}
