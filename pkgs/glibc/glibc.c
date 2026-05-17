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

const pkg pkgs_glibc = {
    .name        = "glibc",
    .version     = "2.40",
    .src         = "https://ftpmirror.gnu.org/gnu/glibc/glibc-2.40.tar.xz",
    .sha256      = "19a890175e9263d748f627993de6f4b1af9cd21e03f080e4bfb3a1fac10205a2",
    .deps        = { .data = NULL, .len = 0 },
    .build_flags = "",
    .build_sys   = BUILD_AUTOTOOLS,
};
PKG_REGISTER(pkgs_glibc);
