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

#include "command_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arena.h"
#include "build.h"
#include "logging.h"
#include "scenicos.h"

int cmd_build(RuntimeOpts *options, int argc, char **argv)
{
    arena *a;

    (void)options;
    if (argc < 2) {
        log(&options->logger, LOG_ERROR, "build", "no package to build.");
        return EXIT_FAILURE;
    }

    a = arena_create(1 << 20);

    if (a == NULL) {
        log(&options->logger, LOG_ERROR, "build", "couldn't allocate arena.");
        return EXIT_FAILURE;
    }

    pkg_refs all = pkgs_all();
    for (size_t i = 0; i < all.len; i++) {
         if (!strcmp(all.data[i]->name, argv[1])) {
            build_pkg_from_def(a, all.data[i]);
            arena_destroy(a);
            return EXIT_SUCCESS;
        }
    }
    logf(&options->logger, LOG_ERROR, "build",
        "no package named [%s] to build.", argv[1]);
    arena_destroy(a);
    return EXIT_FAILURE;
}

