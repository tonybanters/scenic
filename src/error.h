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

#ifndef SCENIC_ERROR_H
#define SCENIC_ERROR_H

#include <stddef.h>

typedef enum {
    RESOLVE_OK = 0,
    RESOLVE_E_CYCLE,
    RESOLVE_E_MISSING_DEP,
    RESOLVE_E_DUPLICATE_NAME,
    RESOLVE_E_OOM,
} resolve_error_kind;

typedef struct {
    resolve_error_kind kind;
    const char        *pkg_name;
    const char        *dep_name;
} resolve_error;

#define RESOLVE_OK_VAL ((resolve_error){ .kind = RESOLVE_OK })

typedef enum {
    FETCH_OK = 0,
    FETCH_E_NETWORK,
    FETCH_E_HASH_MISMATCH,
    FETCH_E_IO,
} fetch_error_kind;

typedef struct {
    fetch_error_kind kind;
    const char      *url;
    const char      *expected_sha;
    const char      *actual_sha;
    int              errno_val;
} fetch_error;

#define FETCH_OK_VAL ((fetch_error){ .kind = FETCH_OK })

typedef enum {
    RUN_OK = 0,
    RUN_E_SPAWN,
    RUN_E_NONZERO,
    RUN_E_IO,
} run_error_kind;

typedef struct {
    run_error_kind kind;
    int            exit_code;
    const char    *log_path;
    int            errno_val;
} run_error;

#define RUN_OK_VAL ((run_error){ .kind = RUN_OK })

typedef enum {
    REALIZE_OK = 0,
    REALIZE_E_FETCH,
    REALIZE_E_BUILD,
    REALIZE_E_SANDBOX,
    REALIZE_E_STORE,
} realize_error_kind;

typedef struct {
    realize_error_kind kind;
    const char        *pkg_name;
    union {
        fetch_error fetch;
        run_error   run;
        int         errno_val;
    } value;
} realize_error;

#define REALIZE_OK_VAL ((realize_error){ .kind = REALIZE_OK })

void resolve_error_print(const resolve_error *e);
void fetch_error_print(const fetch_error *e);
void run_error_print(const run_error *e);
void realize_error_print(const realize_error *e);

#endif
