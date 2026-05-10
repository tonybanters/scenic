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

#include "validate.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static bool is_known_bootloader(const char *name) {
    return strcmp(name, "limine")   == 0
        || strcmp(name, "grub")     == 0
        || strcmp(name, "syslinux") == 0;
}

/**
 * validate() - Run static checks on the system config.
 * @cfg: Configuration to inspect.
 *
 * Covers what resolve() does not: hostname presence, bootloader
 * recognition, non-null kernel/shell pointers, and uniqueness of user
 * and service names. Pkg references (kernel, shells, declared pkgs)
 * are all closure roots — resolve() auto-discovers transitive deps,
 * so no "is in the package list" check is needed. Errors are printed
 * to stderr.
 *
 * Return: true if every check passes.
 */
bool validate(const system_cfg *cfg) {
    bool ok = true;

    if (cfg->hostname == NULL || cfg->hostname[0] == '\0') {
        fprintf(stderr, "scn: hostname is empty\n");
        ok = false;
    }

    if (!is_known_bootloader(cfg->boot.bootloader)) {
        fprintf(stderr, "scn: unknown bootloader '%s'\n", cfg->boot.bootloader);
        ok = false;
    }

    if (cfg->boot.kernel == NULL) {
        fprintf(stderr, "scn: boot.kernel is null\n");
        ok = false;
    }

    for (size_t i = 0; i < cfg->users.len; i++) {
        const user *u = &cfg->users.data[i];

        if (u->name == NULL || u->name[0] == '\0') {
            fprintf(stderr, "scn: user[%zu] has empty name\n", i);
            ok = false;
            continue;
        }

        if (u->shell == NULL) {
            fprintf(stderr, "scn: user '%s' has null shell\n", u->name);
            ok = false;
        }

        for (size_t j = i + 1; j < cfg->users.len; j++) {
            if (strcmp(u->name, cfg->users.data[j].name) == 0) {
                fprintf(stderr, "scn: duplicate user name '%s'\n", u->name);
                ok = false;
            }
        }
    }

    for (size_t i = 0; i < cfg->services.len; i++) {
        const service *s = &cfg->services.data[i];

        if (s->name == NULL || s->name[0] == '\0') {
            fprintf(stderr, "scn: service[%zu] has empty name\n", i);
            ok = false;
            continue;
        }

        for (size_t j = i + 1; j < cfg->services.len; j++) {
            if (strcmp(s->name, cfg->services.data[j].name) == 0) {
                fprintf(stderr, "scn: duplicate service name '%s'\n", s->name);
                ok = false;
            }
        }
    }

    return ok;
}
