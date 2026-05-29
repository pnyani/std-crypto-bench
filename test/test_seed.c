/* SEED: RFC 4269 Section 4, Test Vector #1. */

#include <stdint.h>
#include <stdio.h>
#include "test_utils.h"
#include "seed.h"

void run_seed_tests(void)
{
    FILE          *f = test_open_vectors("test/vectors/seed_ecb.txt");
    test_vector_t  v;
    uint8_t        out[16];

    while (test_load_vector(f, &v)) {
        seed_ctx_t ctx;
        char       enc[32], dec[32];
        snprintf(enc, sizeof(enc), "SEED [%d] encrypt", v.count);
        snprintf(dec, sizeof(dec), "SEED [%d] decrypt", v.count);
        seed_key_expand(v.key, &ctx);
        seed_encrypt(v.pt, out, &ctx);
        test_assert_bytes(enc, v.ct, out, 16);
        seed_decrypt(v.ct, out, &ctx);
        test_assert_bytes(dec, v.pt, out, 16);
    }
    fclose(f);
}
