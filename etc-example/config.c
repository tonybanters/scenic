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

/* /etc/scn/config.c */
#include "scenicos.h"

extern const pkg pkgs_dash;
extern const pkg pkgs_firefox;
extern const pkg pkgs_glibc;
extern const pkg pkgs_linux;
extern const pkg pkgs_neovim;
extern const pkg pkgs_oxwm;

static const pkg *const my_pkgs[] = {
    &pkgs_glibc,
    &pkgs_linux,
    &pkgs_neovim,
    &pkgs_oxwm,
    &pkgs_firefox,
};

static const char *const sshd_after[]  = { "network" };
static const service my_services[] = {
    { .name = "sshd", .enabled = true,
      .after = { .data = sshd_after, .len = 1 } },
};

static const char *const tony_groups[] = { "wheel", "video", "audio" };
static const user my_users[] = {
    { .name   = "tony",
      .shell  = &pkgs_dash,
      .home   = "/home/tony",
      .groups = { .data = tony_groups, .len = 3 } },
};

const system_cfg CFG = SYSTEM_CFG_INIT(
    .hostname = "scenic-btw",
    .boot = {
        .bootloader    = "limine",
        .kernel        = &pkgs_linux,
        .kernel_params = "quiet loglevel=3",
    },
    .pkgs     = { .data = my_pkgs,     .len = 5 },
    .services = { .data = my_services, .len = 1 },
    .users    = { .data = my_users,    .len = 1 },
);
