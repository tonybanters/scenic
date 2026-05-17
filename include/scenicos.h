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

#ifndef SCENICOS_H
#define SCENICOS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define SCENICOS_SCHEMA_VERSION 1

typedef struct {
    const char *const *data;
    size_t             len;
} strs;

typedef struct pkg pkg;

typedef struct {
    const pkg *const *data;
    size_t            len;
} pkg_refs;

typedef enum {
    BUILD_AUTOTOOLS = 1,
    BUILD_CMAKE     = 2,
    BUILD_MESON     = 3,
    BUILD_MAKE      = 4,
    BUILD_CARGO     = 5,
    BUILD_GO        = 6,
    BUILD_ZIG       = 7,
    BUILD_SHELL     = 99,
} build_system;

struct pkg {
    const char  *name;
    const char  *version;
    const char  *src;
    const char  *sha256;
    pkg_refs     deps;
    const char  *build_flags;
    build_system build_sys;
};

/* each pkg.c registers itself, the linker gathers entires into pkgs_registry */
#define PKG_REGISTER(p) \
    static const pkg *const _reg_##p \
        __attribute__((used, section("pkgs_registry"))) = &(p)

extern const pkg *const __start_pkgs_registry[];
extern const pkg *const __stop_pkgs_registry[];

static inline pkg_refs pkgs_all(void) {
    return (pkg_refs){
        .data = __start_pkgs_registry,
        .len  = (size_t)(__stop_pkgs_registry - __start_pkgs_registry),
    };
}

typedef struct {
    const char *name;
    bool        enabled;
    strs        after;
} service;

typedef struct {
    const service *data;
    size_t         len;
} services;

typedef struct {
    const char *name;
    const pkg  *shell;
    const char *home;
    strs        groups;
} user;

typedef struct {
    const user *data;
    size_t      len;
} users;

typedef struct {
    const char *bootloader;
    const pkg  *kernel;
    const char *kernel_params;
} boot_cfg;

typedef struct {
    uint32_t    schema_version;
    const char *hostname;
    boot_cfg    boot;
    pkg_refs    pkgs;
    services    services;
    users       users;
} system_cfg;

#define SYSTEM_CFG_INIT(...) { \
    .schema_version = SCENICOS_SCHEMA_VERSION, \
    __VA_ARGS__ \
}

#endif
