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

static const pkg *const dash_deps[] = { &pkgs_glibc };

const pkg pkgs_dash = {
    .name        = "dash",
    .version     = "0.5.12",
    .src         = "http://gondor.apana.org.au/~herbert/dash/files/dash-0.5.12.tar.gz",
    .sha256      = "6a474ac46e8b0b32916c4c60df694c82058d3297d8b385b74508030ca4a8f28a",
    .deps        = { .data = dash_deps, .len = sizeof(dash_deps) / sizeof(dash_deps[0]) },
    .build_flags = "",
    .build_sys   = BUILD_AUTOTOOLS,
};
PKG_REGISTER(pkgs_dash);
