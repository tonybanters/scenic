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

#include "lua.h"
#include "../glibc/glibc.h"

static const pkg *const lua_deps[] = { &pkgs_glibc };

const pkg pkgs_lua = {
    .name        = "lua",
    .version     = "5.4.7",
    .src         = "https://www.lua.org/ftp/lua-5.4.7.tar.gz",
    .sha256      = "0000000000000000000000000000000000000000000000000000000000000000",
    .deps        = { .data = lua_deps, .len = sizeof(lua_deps) / sizeof(lua_deps[0]) },
    .build_flags = "",
    .build_sys   = BUILD_MAKE,
};
