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

#include "hash.h"

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static const uint32_t K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

#define ROTR(x, n)   (((x) >> (n)) | ((x) << (32 - (n))))
#define CH(x, y, z)  (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define BSIG0(x)     (ROTR(x,  2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define BSIG1(x)     (ROTR(x,  6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define SSIG0(x)     (ROTR(x,  7) ^ ROTR(x, 18) ^ ((x) >>  3))
#define SSIG1(x)     (ROTR(x, 17) ^ ROTR(x, 19) ^ ((x) >> 10))

static void sha256_compress(sha256_ctx *ctx, const uint8_t block[64]) {
    uint32_t W[64];
    uint32_t a, b, c, d, e, f, g, h;
    uint32_t T1, T2;

    for (int i = 0; i < 16; i++) {
        W[i] = ((uint32_t)block[i * 4]     << 24) |
               ((uint32_t)block[i * 4 + 1] << 16) |
               ((uint32_t)block[i * 4 + 2] <<  8) |
               ((uint32_t)block[i * 4 + 3]);
    }
    for (int i = 16; i < 64; i++) {
        W[i] = SSIG1(W[i - 2]) + W[i - 7] + SSIG0(W[i - 15]) + W[i - 16];
    }

    a = ctx->state[0]; b = ctx->state[1]; c = ctx->state[2]; d = ctx->state[3];
    e = ctx->state[4]; f = ctx->state[5]; g = ctx->state[6]; h = ctx->state[7];

    for (int i = 0; i < 64; i++) {
        T1 = h + BSIG1(e) + CH(e, f, g) + K[i] + W[i];
        T2 = BSIG0(a) + MAJ(a, b, c);
        h = g; g = f; f = e; e = d + T1;
        d = c; c = b; b = a; a = T1 + T2;
    }

    ctx->state[0] += a; ctx->state[1] += b; ctx->state[2] += c; ctx->state[3] += d;
    ctx->state[4] += e; ctx->state[5] += f; ctx->state[6] += g; ctx->state[7] += h;
}

/**
 * sha256_init() - Initialize a SHA-256 hashing context.
 * @ctx: Context to initialize.
 */
void sha256_init(sha256_ctx *ctx) {
    ctx->state[0] = 0x6a09e667; ctx->state[1] = 0xbb67ae85;
    ctx->state[2] = 0x3c6ef372; ctx->state[3] = 0xa54ff53a;
    ctx->state[4] = 0x510e527f; ctx->state[5] = 0x9b05688c;
    ctx->state[6] = 0x1f83d9ab; ctx->state[7] = 0x5be0cd19;
    ctx->bitcount = 0;
    ctx->buffer_used = 0;
}

/**
 * sha256_update() - Feed data into a SHA-256 context.
 * @ctx: Context previously initialized with sha256_init().
 * @data: Bytes to hash.
 * @len: Length of @data in bytes.
 */
void sha256_update(sha256_ctx *ctx, const void *data, size_t len) {
    const uint8_t *p = data;
    while (len > 0) {
        size_t n = 64 - ctx->buffer_used;
        if (n > len) n = len;
        memcpy(ctx->buffer + ctx->buffer_used, p, n);
        ctx->buffer_used += n;
        p   += n;
        len -= n;
        if (ctx->buffer_used == 64) {
            sha256_compress(ctx, ctx->buffer);
            ctx->bitcount += 512;
            ctx->buffer_used = 0;
        }
    }
}

/**
 * sha256_final() - Pad and finalize, writing the digest.
 * @ctx: Context to finalize. Caller must not reuse without re-init.
 * @out: 32-byte buffer to receive the digest.
 */
void sha256_final(sha256_ctx *ctx, uint8_t out[32]) {
    ctx->bitcount += (uint64_t)ctx->buffer_used * 8;
    ctx->buffer[ctx->buffer_used++] = 0x80;
    if (ctx->buffer_used > 56) {
        while (ctx->buffer_used < 64) ctx->buffer[ctx->buffer_used++] = 0;
        sha256_compress(ctx, ctx->buffer);
        ctx->buffer_used = 0;
    }
    while (ctx->buffer_used < 56) ctx->buffer[ctx->buffer_used++] = 0;
    for (int i = 7; i >= 0; i--) {
        ctx->buffer[ctx->buffer_used++] = (uint8_t)(ctx->bitcount >> (i * 8));
    }
    sha256_compress(ctx, ctx->buffer);

    for (int i = 0; i < 8; i++) {
        out[i * 4]     = (uint8_t)(ctx->state[i] >> 24);
        out[i * 4 + 1] = (uint8_t)(ctx->state[i] >> 16);
        out[i * 4 + 2] = (uint8_t)(ctx->state[i] >>  8);
        out[i * 4 + 3] = (uint8_t)(ctx->state[i]);
    }
}

/**
 * sha256_hash() - One-shot SHA-256 over a single buffer.
 * @data: Bytes to hash.
 * @len: Length of @data in bytes.
 * @out: 32-byte buffer to receive the digest.
 */
void sha256_hash(const void *data, size_t len, uint8_t out[32]) {
    sha256_ctx ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, data, len);
    sha256_final(&ctx, out);
}

/**
 * sha256_hex() - Hex-encode a SHA-256 digest.
 * @digest: 32-byte digest.
 * @out: 65-byte buffer to receive the lowercase hex string and NUL.
 */
void sha256_hex(const uint8_t digest[32], char out[65]) {
    static const char hex[] = "0123456789abcdef";
    for (int i = 0; i < 32; i++) {
        out[i * 2]     = hex[digest[i] >> 4];
        out[i * 2 + 1] = hex[digest[i] & 0xf];
    }
    out[64] = '\0';
}

/**
 * sha256_verify_file() - Compare a file's SHA-256 against an expected hex.
 * @path: Filesystem path to read.
 * @expected_hex: Expected lowercase hex digest, or NULL to skip comparison.
 * @actual_hex: Optional 65-byte buffer to receive the computed hex digest.
 *
 * Return: true if the file hashes to @expected_hex (or @expected_hex is NULL
 * and the file was readable); false on mismatch or I/O error.
 */
bool sha256_verify_file(
    const char *path,
    const char *expected_hex,
    char        actual_hex[65])
{
    int fd = open(path, O_RDONLY | O_CLOEXEC);
    if (fd < 0) return false;

    sha256_ctx ctx;
    sha256_init(&ctx);

    uint8_t buf[8192];
    for (;;) {
        ssize_t n = read(fd, buf, sizeof(buf));
        if (n < 0) {
            close(fd);
            return false;
        }
        if (n == 0) break;
        sha256_update(&ctx, buf, (size_t)n);
    }
    close(fd);

    uint8_t digest[32];
    sha256_final(&ctx, digest);

    char hex[65];
    sha256_hex(digest, hex);
    if (actual_hex != NULL) memcpy(actual_hex, hex, 65);

    if (expected_hex == NULL) return true;
    return strcmp(hex, expected_hex) == 0;
}
