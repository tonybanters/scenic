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

#include "sandbox.h"

#include <errno.h>
#include <fcntl.h>
#include <sched.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * sandbox_setup() - Build a hermetic sandbox at @sandbox_root.
 * @sandbox_root: Directory the sandbox is constructed under.
 * @deps: Direct build deps of the package being built.
 * @resolved_deps: Resolved entries paralleling the system's pkg list.
 * @src_dir: Source tree to bind-mount as the build root inside.
 *
 * Read-only bind mounts each dep's store path; provides /dev/null,
 * /dev/urandom, fresh procfs, a private tmpfs, and the source tree
 * at /build. No network, no host /home, /etc, or /root.
 *
 * Return: 0 on success, errno value on failure.
 */
int sandbox_setup(
    const char     *sandbox_root,
    const pkg_refs *deps,
    const resolved *resolved_deps,
    const char     *src_dir
) {
    (void)sandbox_root;
    (void)deps;
    (void)resolved_deps;
    (void)src_dir;
    return ENOSYS;
}

/**
 * sandbox_teardown() - Undo a sandbox built by sandbox_setup().
 * @sandbox_root: Directory previously passed to sandbox_setup().
 */
void sandbox_teardown(const char *sandbox_root) {
    if (sandbox_root == NULL) return;
}
