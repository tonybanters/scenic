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

#include "error.h"

#include <stdio.h>
#include <string.h>

void resolve_error_print(const resolve_error *e) {
    switch (e->kind) {
        case RESOLVE_OK:
            return;
        case RESOLVE_E_CYCLE:
            fprintf(stderr, "scn: dependency cycle detected at '%s'\n", e->pkg_name);
            return;
        case RESOLVE_E_MISSING_DEP:
            fprintf(stderr, "scn: package '%s' depends on '%s' which is not in the package list\n", e->pkg_name, e->dep_name);
            return;
        case RESOLVE_E_DUPLICATE_NAME:
            fprintf(stderr, "scn: duplicate package name '%s' (two recipes export the same name)\n", e->pkg_name);
            return;
        case RESOLVE_E_OOM:
            fprintf(stderr, "scn: out of memory while resolving packages\n");
            return;
    }
}

void fetch_error_print(const fetch_error *e) {
    switch (e->kind) {
        case FETCH_OK:
            return;
        case FETCH_E_NETWORK:
            fprintf(stderr, "scn: network error fetching %s: %s\n", e->url, strerror(e->errno_val));
            return;
        case FETCH_E_HASH_MISMATCH:
            fprintf(stderr, "scn: hash mismatch for %s\n", e->url);
            fprintf(stderr, "  expected: %s\n", e->expected_sha);
            fprintf(stderr, "  actual:   %s\n", e->actual_sha);
            return;
        case FETCH_E_IO:
            fprintf(stderr, "scn: I/O error fetching %s: %s\n", e->url, strerror(e->errno_val));
            return;
    }
}

void run_error_print(const run_error *e) {
    switch (e->kind) {
        case RUN_OK:
            return;
        case RUN_E_SPAWN:
            fprintf(stderr, "scn: failed to spawn process: %s\n",
                    strerror(e->errno_val));
            return;
        case RUN_E_NONZERO:
            fprintf(stderr, "scn: process exited %d (log: %s)\n",
                    e->exit_code, e->log_path);
            return;
        case RUN_E_IO:
            fprintf(stderr, "scn: I/O error: %s\n", strerror(e->errno_val));
            return;
    }
}

void realize_error_print(const realize_error *e) {
    switch (e->kind) {
        case REALIZE_OK:
            return;
        case REALIZE_E_FETCH:
            fprintf(stderr, "scn: failed to realize '%s' (fetch):\n  ", e->pkg_name);
            fetch_error_print(&e->value.fetch);
            return;
        case REALIZE_E_BUILD:
            fprintf(stderr, "scn: failed to realize '%s' (build):\n  ", e->pkg_name);
            run_error_print(&e->value.run);
            return;
        case REALIZE_E_SANDBOX:
            fprintf(stderr, "scn: failed to set up sandbox for '%s': %s\n",
                    e->pkg_name, strerror(e->value.errno_val));
            return;
        case REALIZE_E_STORE:
            fprintf(stderr, "scn: store error for '%s': %s\n",
                    e->pkg_name, strerror(e->value.errno_val));
            return;
    }
}
