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

#include "fetch.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "hash.h"
#include "run.h"

/**
 * fetch() - Download a package's source tarball and verify its hash.
 * @p: Package whose @src URL and @sha256 are used.
 * @dest_path: Destination path on disk.
 *
 * Shells out to curl(1) so scn does not link libcurl. The downloaded
 * file's sha256 is compared against @p->sha256.
 *
 * Return: FETCH_OK_VAL on success, a tagged error otherwise.
 */
fetch_error fetch(const pkg *p, const char *dest_path) {
    // TODO: replace this with execvpe or libcurl
    char *const argv[] = {
        "/usr/bin/curl",
        "-fsSL",
        "--retry", "3",
        "--retry-delay", "2",
        "-o", (char *)dest_path,
        (char *)p->src,
        NULL,
    };

    char *const envp[] = { NULL };

    char log_path[4096];
    int n = snprintf(log_path, sizeof(log_path), "%s.fetch.log", dest_path);
    if (n < 0 || (size_t)n >= sizeof(log_path)) {
        return (fetch_error){
            .kind = FETCH_E_IO,
            .url = p->src,
            .errno_val = ENAMETOOLONG,
        };
    }

    run_error rerr = run("/usr/bin/curl", argv, envp, NULL, log_path);
    if (rerr.kind != RUN_OK) {
        fetch_error fe = {
            .kind = FETCH_E_NETWORK,
            .url = p->src,
            .errno_val = 0,
        };
        if (rerr.kind == RUN_E_SPAWN || rerr.kind == RUN_E_IO) {
            fe.kind = FETCH_E_IO;
            fe.errno_val = rerr.errno_val;
        }
        return fe;
    }

    char actual[65] = {0};
    if (!sha256_verify_file(dest_path, p->sha256, actual)) {
        return (fetch_error){
            .kind = FETCH_E_HASH_MISMATCH,
            .url = p->src,
            .expected_sha = p->sha256,
            .actual_sha = strdup(actual),
        };
    }

    return FETCH_OK_VAL;
}
