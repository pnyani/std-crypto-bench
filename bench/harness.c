#include "harness.h"

#include <valgrind/callgrind.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void bench_run(const bench_algo_t *algo,
               size_t data_bytes,
               int warmup,
               int repeat,
               bench_dir_t dir)
{
    size_t  bs      = (size_t)algo->block_size;
    size_t  nblocks = data_bytes / bs;
    uint8_t *in     = malloc(data_bytes);
    uint8_t *out    = malloc(data_bytes);

    if (!in || !out) {
        fprintf(stderr, "[bench] malloc failed (%s)\n", algo->name);
        free(in);
        free(out);
        return;
    }

    {
        size_t i;
        for (i = 0; i < data_bytes; i++)
            in[i] = (uint8_t)(i & 0xFF);
    }

    /* key expansion outside the measured region */
    algo->setup(algo->key, algo->ctx);

    {
        int w;
        for (w = 0; w < warmup; w++) {
            if (dir == BENCH_ENCRYPT)
                algo->encrypt_blocks(in, out, nblocks, algo->ctx);
            else
                algo->decrypt_blocks(in, out, nblocks, algo->ctx);
        }
    }

    CALLGRIND_ZERO_STATS;
    CALLGRIND_START_INSTRUMENTATION;

    {
        int r;
        if (dir == BENCH_ENCRYPT) {
            for (r = 0; r < repeat; r++)
                algo->encrypt_blocks(in, out, nblocks, algo->ctx);
        } else {
            for (r = 0; r < repeat; r++)
                algo->decrypt_blocks(in, out, nblocks, algo->ctx);
        }
    }

    CALLGRIND_STOP_INSTRUMENTATION;

    free(in);
    free(out);
}
