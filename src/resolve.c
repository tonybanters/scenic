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

#include "resolve.h"

#include <stdalign.h>
#include <stdint.h>
#include <string.h>

#include "store.h"

typedef enum {
    UNVISITED = 0,
    VISITING,
    VISITED,
} visit_state;

typedef struct closure_node closure_node;
struct closure_node {
    const pkg    *p;
    closure_node *next;
};

static bool closure_contains(const closure_node *head, const pkg *p) {
    for (const closure_node *n = head; n != NULL; n = n->next) {
        if (n->p == p) return true;
    }
    return false;
}

static bool closure_collect(
        arena         *a,
        const pkg     *p,
        closure_node **head,
        size_t        *count
    ) {
    if (closure_contains(*head, p)) return true;

    closure_node *node = arena_alloc(a, sizeof(closure_node), alignof(closure_node));
    if (node == NULL) return false;
    node->p    = p;
    node->next = *head;
    *head      = node;
    (*count)++;

    for (size_t i = 0; i < p->deps.len; i++) {
        if (!closure_collect(a, p->deps.data[i], head, count)) return false;
    }
    return true;
}

static size_t closure_index(
    const pkg *const *closure,
    size_t            n,
    const pkg        *target)
{
    for (size_t i = 0; i < n; i++) {
        if (closure[i] == target) return i;
    }
    return SIZE_MAX;
}

static resolve_error visit(
    arena             *a,
    const pkg *const  *closure,
    size_t             n_closure,
    size_t             idx,
    visit_state       *state,
    resolved          *out,
    size_t            *out_len)
{
    switch (state[idx]) {
        case VISITED:  return RESOLVE_OK_VAL;
        case VISITING: return (resolve_error){
            .kind = RESOLVE_E_CYCLE,
            .pkg_name = closure[idx]->name,
        };
        case UNVISITED: break;
    }

    state[idx] = VISITING;
    const pkg *p = closure[idx];

    for (size_t i = 0; i < p->deps.len; i++) {
        size_t dep_idx = closure_index(closure, n_closure, p->deps.data[i]);
        resolve_error err = visit(a, closure, n_closure, dep_idx, state, out, out_len);
        if (err.kind != RESOLVE_OK) return err;
    }

    out[*out_len].def        = p;
    out[*out_len].store_path = store_path_compute(a, p, out, *out_len);
    (*out_len)++;
    state[idx] = VISITED;
    return RESOLVE_OK_VAL;
}

/**
 * resolve() - Compute the build closure and topologically sort it.
 * @a: Arena holding the resulting closure, visit-state, and resolved arrays.
 * @cfg: System configuration providing the package roots.
 * @out: Filled with topologically-sorted &resolved entries on success.
 *
 * Roots are everything the config references directly: @cfg->pkgs entries,
 * @cfg->boot.kernel, and each @cfg->users[i].shell. Transitive deps are
 * auto-discovered into the closure. The output is in post-order so
 * &realize can iterate it directly.
 *
 * Return: RESOLVE_OK_VAL on success, a tagged error otherwise.
 */
resolve_error resolve(
    arena              *a,
    const system_cfg   *cfg,
    resolved_list      *out)
{
    for (size_t i = 0; i < cfg->pkgs.len; i++) {
        for (size_t j = i + 1; j < cfg->pkgs.len; j++) {
            if (strcmp(cfg->pkgs.data[i]->name, cfg->pkgs.data[j]->name) == 0) {
                return (resolve_error){
                    .kind = RESOLVE_E_DUPLICATE_NAME,
                    .pkg_name = cfg->pkgs.data[i]->name,
                };
            }
        }
    }

    closure_node *head = NULL;
    size_t n_closure = 0;
    for (size_t i = 0; i < cfg->pkgs.len; i++) {
        if (!closure_collect(a, cfg->pkgs.data[i], &head, &n_closure)) {
            return (resolve_error){ .kind = RESOLVE_E_OOM };
        }
    }
    if (cfg->boot.kernel != NULL) {
        if (!closure_collect(a, cfg->boot.kernel, &head, &n_closure)) {
            return (resolve_error){ .kind = RESOLVE_E_OOM };
        }
    }
    for (size_t i = 0; i < cfg->users.len; i++) {
        const pkg *shell = cfg->users.data[i].shell;
        if (shell != NULL) {
            if (!closure_collect(a, shell, &head, &n_closure)) {
                return (resolve_error){ .kind = RESOLVE_E_OOM };
            }
        }
    }

    const pkg **closure = arena_alloc(a, n_closure * sizeof(const pkg *),
                                       alignof(const pkg *));
    visit_state *state  = arena_alloc(a, n_closure * sizeof(visit_state),
                                       alignof(visit_state));
    resolved    *res    = arena_alloc(a, n_closure * sizeof(resolved),
                                       alignof(resolved));
    if (closure == NULL || state == NULL || res == NULL) {
        return (resolve_error){ .kind = RESOLVE_E_OOM };
    }

    {
        size_t i = 0;
        for (closure_node *n = head; n != NULL; n = n->next) {
            closure[i++] = n->p;
        }
    }

    size_t res_len = 0;
    for (size_t i = 0; i < n_closure; i++) {
        resolve_error err = visit(a, closure, n_closure, i, state, res, &res_len);
        if (err.kind != RESOLVE_OK) return err;
    }

    out->data = res;
    out->len  = res_len;
    return RESOLVE_OK_VAL;
}
