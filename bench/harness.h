#ifndef BENCH_HARNESS_H
#define BENCH_HARNESS_H

/* Warmup runs execute outside the instrumented region. */

#include <stdint.h>
#include <stddef.h>

typedef enum {
    BENCH_ENCRYPT = 0,
    BENCH_DECRYPT = 1
} bench_dir_t;

typedef struct {
    const char    *name;
    int            key_size;
    int            block_size;
    void         (*setup)  (const uint8_t *key, void *ctx);
    void         (*encrypt_blocks)(const uint8_t *in,
                                   uint8_t *out,
                                   size_t blocks,
                                   const void *ctx);
    void         (*decrypt_blocks)(const uint8_t *in,
                                   uint8_t *out,
                                   size_t blocks,
                                   const void *ctx);
    void          *ctx;
    const uint8_t *key;
} bench_algo_t;

void bench_run(const bench_algo_t *algo,
               size_t data_bytes,
               int warmup,
               int repeat,
               bench_dir_t dir);

#endif /* BENCH_HARNESS_H */
