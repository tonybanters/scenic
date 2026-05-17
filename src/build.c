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

#include "build.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "fetch.h"
#include "run.h"
#include "sandbox.h"
#include "store.h"

#ifdef USE_LOCAL_DIRS
    #define WORK_ROOT ".scn-runtime/work"
#else
    #define WORK_ROOT "/var/lib/scn/work"
#endif

/**
 * build_pkg() - Realize one package into the store.
 * @a: Arena for path scratch space.
 * @p: Package to build.
 * @all_pkgs: Full package list for the system (currently unused).
 * @resolved_pkgs: Topologically-sorted resolved entries.
 * @pkg_idx: Index of @p within @resolved_pkgs.
 *
 * Idempotent: returns success immediately if the store path already
 * exists. Otherwise fetches the source tarball, extracts it, sets up
 * a sandbox containing the resolved deps, runs the build, and atomically
 * moves the output into the store.
 *
 * Return: REALIZE_OK_VAL on success, a tagged error otherwise.
 */
realize_error build_pkg(
    arena              *a,
    const pkg          *p,
    const pkg_refs     *all_pkgs,
    const resolved     *resolved_pkgs,
    size_t              pkg_idx)
{
    (void)all_pkgs;

    const char *out = resolved_pkgs[pkg_idx].out;

    if (store_path_exists(out)) {
        return REALIZE_OK_VAL;
    }

    const char *base = strrchr(out, '/');
    base = (base != NULL) ? base + 1 : out;

    char *work_dir     = arena_sprintf(a, "%s/%s", WORK_ROOT, base);
    char *src_tarball  = arena_sprintf(a, "%s/source", work_dir);
    char *src_dir      = arena_sprintf(a, "%s/build", work_dir);
    char *sandbox_root = arena_sprintf(a, "%s/sandbox", work_dir);
    char *log_path     = arena_sprintf(a, "%s/build.log", work_dir);
    char *out_dir      = arena_sprintf(a, "%s/out", work_dir);

    if (work_dir == NULL || src_tarball == NULL || src_dir == NULL || sandbox_root == NULL || log_path == NULL || out_dir == NULL) {
        return (realize_error){
            .kind = REALIZE_E_STORE,
            .pkg_name = p->name,
            .value.errno_val = ENOMEM,
        };
    }

    if (mkdir(WORK_ROOT, 0755) < 0 && errno != EEXIST) {
        return (realize_error){ .kind = REALIZE_E_STORE, .pkg_name = p->name,
                                .value.errno_val = errno };
    }
    if (mkdir(work_dir, 0755) < 0 && errno != EEXIST) {
        return (realize_error){ .kind = REALIZE_E_STORE, .pkg_name = p->name,
                                .value.errno_val = errno };
    }

    fetch_error ferr = fetch(p, src_tarball);
    if (ferr.kind != FETCH_OK) {
        return (realize_error){
            .kind = REALIZE_E_FETCH,
            .pkg_name = p->name,
            .value.fetch = ferr,
        };
    }

    if (mkdir(src_dir, 0755) < 0 && errno != EEXIST) {
        return (realize_error){ .kind = REALIZE_E_STORE, .pkg_name = p->name,
                                .value.errno_val = errno };
    }

    char *const tar_argv[] = {
        "tar", "-xf", src_tarball, "-C", src_dir, "--strip-components=1",
        NULL,
    };
    char *const tar_envp[] = { NULL };

    run_error tar_err = run(tar_argv, tar_envp, NULL, log_path);
    if (tar_err.kind != RUN_OK) {
        return (realize_error){
            .kind = REALIZE_E_BUILD,
            .pkg_name = p->name,
            .value.run = tar_err,
        };
    }

    int src = sandbox_setup(sandbox_root, &p->deps, resolved_pkgs, src_dir);
    if (src != 0) {
        return (realize_error){
            .kind = REALIZE_E_SANDBOX,
            .pkg_name = p->name,
            .value.errno_val = src,
        };
    }

    sandbox_teardown(sandbox_root);

    int rc = store_install(out_dir, out);
    if (rc != 0) {
        return (realize_error){
            .kind = REALIZE_E_STORE,
            .pkg_name = p->name,
            .value.errno_val = rc,
        };
    }

    return REALIZE_OK_VAL;
}

bool build_pkg_from_def(arena *a, const pkg *def)
{
    resolved_list resolved_pkgs;
    system_cfg cfg = { .pkgs = pkgs_all() };

    resolve_error rerr = resolve(a, &cfg, &resolved_pkgs);
    if (rerr.kind != RESOLVE_OK) {
        fprintf(stderr, "%s : %s\n", rerr.pkg_name, rerr.dep_name);
        return false;
    }

    bool found = false;
    size_t def_idx;
    for (size_t i = 0; i < resolved_pkgs.len; i++) {
        if (resolved_pkgs.data[i].def != def) continue;
        def_idx = i;
        found = true;
        break;
    }
    if (!found) {
        fprintf(stderr, "something went wrong :(\n");
        return false;
    }

    printf("building: [%s]\n", resolved_pkgs.data[def_idx].out);

    realize_error err = build_pkg(
        a,
        resolved_pkgs.data[def_idx].def,
        &cfg.pkgs,
        resolved_pkgs.data,
        def_idx);
    if (err.kind != REALIZE_OK) {
        realize_error_print(&err);
        return false;
    }

    return true;
}
