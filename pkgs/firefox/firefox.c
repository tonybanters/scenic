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

static const pkg *const firefox_deps[] = { &pkgs_glibc };

const pkg pkgs_firefox = {
    .name        = "firefox",
    .version     = "133.0",
    .src         = "https://archive.mozilla.org/pub/firefox/releases/133.0/source/firefox-133.0.source.tar.xz",
    .sha256      = "492b2c9a3b6d215e38ce490624e8b2b9473419accdeaddb24ba00bc6adc3cc60",
    .deps        = { .data = firefox_deps, .len = sizeof(firefox_deps) / sizeof(firefox_deps[0]) },
    .build_flags = "",
    .build_sys   = BUILD_SHELL,
};
PKG_REGISTER(pkgs_firefox);
