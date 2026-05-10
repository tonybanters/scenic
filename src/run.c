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

#include "run.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/**
 * run() - Fork+exec a command with stdout/stderr captured to a log.
 * @cmd: Absolute path to the executable.
 * @argv: NULL-terminated argv. argv[0] is conventionally @cmd.
 * @envp: NULL-terminated envp. Pass {NULL} for an empty environment.
 * @cwd: Working directory for the child, or NULL to inherit.
 * @log_path: File path that captures the child's stdout and stderr.
 *
 * Stdin is redirected to /dev/null. The child is reaped before return.
 *
 * Return: RUN_OK_VAL on exit code 0; RUN_E_NONZERO with @exit_code and
 * @log_path filled; RUN_E_SPAWN or RUN_E_IO with @errno_val filled.
 */
run_error run(
    const char  *cmd,
    char *const  argv[],
    char *const  envp[],
    const char  *cwd,
    const char  *log_path)
{
    run_error err = RUN_OK_VAL;

    int log_fd = open(log_path, O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0644);
    if (log_fd < 0) {
        err.kind = RUN_E_IO;
        err.errno_val = errno;
        return err;
    }

    pid_t pid = fork();
    if (pid < 0) {
        err.kind = RUN_E_SPAWN;
        err.errno_val = errno;
        close(log_fd);
        return err;
    }

    if (pid == 0) {
        if (cwd != NULL && chdir(cwd) < 0) {
            dprintf(log_fd, "run: chdir(%s) failed: %s\n", cwd, strerror(errno));
            _exit(127);
        }

        if (dup2(log_fd, STDOUT_FILENO) < 0 || dup2(log_fd, STDERR_FILENO) < 0) {
            _exit(127);
        }
        close(log_fd);

        int devnull = open("/dev/null", O_RDONLY | O_CLOEXEC);
        if (devnull >= 0) {
            dup2(devnull, STDIN_FILENO);
            close(devnull);
        }

        execve(cmd, argv, envp);
        dprintf(STDERR_FILENO, "run: execve(%s) failed: %s\n", cmd, strerror(errno));
        _exit(127);
    }

    close(log_fd);

    int status = 0;
    while (waitpid(pid, &status, 0) < 0) {
        if (errno == EINTR) continue;
        err.kind = RUN_E_IO;
        err.errno_val = errno;
        return err;
    }

    int code;
    if (WIFEXITED(status)) {
        code = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
        code = 128 + WTERMSIG(status);
    } else {
        err.kind = RUN_E_IO;
        err.errno_val = EINVAL;
        return err;
    }

    if (code != 0) {
        err.kind = RUN_E_NONZERO;
        err.exit_code = code;
        err.log_path = log_path;
        return err;
    }

    return RUN_OK_VAL;
}
