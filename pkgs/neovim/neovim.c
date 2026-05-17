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

#include "scenicos.h"

extern const pkg pkgs_glibc;
extern const pkg pkgs_lua;

static const pkg *const neovim_deps[] = {
    &pkgs_glibc,
    &pkgs_lua,
};

const pkg pkgs_neovim = {
    .name        = "neovim",
    .version     = "0.10.2",
    .src         = "https://github.com/neovim/neovim/archive/refs/tags/v0.10.2.tar.gz",
    .sha256      = "546cb2da9fffbb7e913261344bbf4cf1622721f6c5a67aa77609e976e78b8e89",
    .deps        = { .data = neovim_deps,
                     .len = sizeof(neovim_deps) / sizeof(neovim_deps[0]) },
    .build_flags = "",
    .build_sys   = BUILD_CMAKE,
};
PKG_REGISTER(pkgs_neovim);
