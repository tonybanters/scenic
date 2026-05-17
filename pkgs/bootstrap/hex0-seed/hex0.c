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

const pkg pkgs_hex0 = {
    .name = "hex0-seed",
    .version = "779e0105b6ea7a8f6d860cdcd7853ec33481fa5b",
    .src = "https://github.com/oriansj/bootstrap-seeds/archive/779e0105b6ea7a8f6d860cdcd7853ec33481fa5b.tar.gz",
    .sha256 = "5c0b88037a78beecc29293bfdf9a3e37d29ab4ab33a5b4f5771f0ab802257a81",
};
PKG_REGISTER(pkgs_hex0);
