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

#include "firefox.h"
#include "../glibc/glibc.h"

static const pkg *const firefox_deps[] = { &pkgs_glibc };

const pkg pkgs_firefox = {
    .name        = "firefox",
    .version     = "133.0",
    .src         = "https://archive.mozilla.org/pub/firefox/releases/133.0/source/firefox-133.0.source.tar.xz",
    .sha256      = "0000000000000000000000000000000000000000000000000000000000000000",
    .deps        = { .data = firefox_deps, .len = sizeof(firefox_deps) / sizeof(firefox_deps[0]) },
    .build_flags = "",
    .build_sys   = BUILD_SHELL,
};
