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

#include "neovim.h"
#include "../glibc/glibc.h"
#include "../lua/lua.h"

static const pkg *const neovim_deps[] = {
    &pkgs_glibc,
    &pkgs_lua,
};

const pkg pkgs_neovim = {
    .name        = "neovim",
    .version     = "0.10.2",
    .src         = "https://github.com/neovim/neovim/archive/refs/tags/v0.10.2.tar.gz",
    .sha256      = "0000000000000000000000000000000000000000000000000000000000000000",
    .deps        = { .data = neovim_deps,
                     .len = sizeof(neovim_deps) / sizeof(neovim_deps[0]) },
    .build_flags = "",
    .build_sys   = BUILD_CMAKE,
};
