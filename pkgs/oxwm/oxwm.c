#include "scenicos.h"

extern const pkg pkgs_glibc;
extern const pkg pkgs_lua;

static const pkg *const oxwm_deps[] = {
    &pkgs_glibc,
    &pkgs_lua,
};

const pkg pkgs_oxwm = {
    .name        = "oxwm",
    .version     = "0.11.4",
    .src         = "https://github.com/tonybanters/oxwm/archive/refs/tags/0.11.4.tar.gz",
    .sha256      = "9d6fbde0cfd73706319c294c140c7d15faf4128b7c1a040fe3c4528778477035",
    .deps        = { .data = oxwm_deps, .len = sizeof(oxwm_deps) / sizeof(oxwm_deps[0]) },
    .build_flags = "",
    .build_sys   = BUILD_ZIG,
};
PKG_REGISTER(pkgs_oxwm);
