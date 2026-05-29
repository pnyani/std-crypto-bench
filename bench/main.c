#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "aes.h"
#include "aes_variant.h"
#include "aria.h"
#include "seed.h"
#include "des.h"
#include "3des.h"

static void w_aes128_setup(const uint8_t *k, void *c)
    { aes128_key_expand(k, (aes128_ctx_t *)c); }
static void w_aes128_enc(const uint8_t *i, uint8_t *o, size_t b, const void *c)
    { aes128_encrypt_blocks(i, o, b, (const aes128_ctx_t *)c); }
static void w_aes128_dec(const uint8_t *i, uint8_t *o, size_t b, const void *c)
    { aes128_decrypt_blocks(i, o, b, (const aes128_ctx_t *)c); }

static void w_aes256_setup(const uint8_t *k, void *c)
    { aes256_key_expand(k, (aes256_ctx_t *)c); }
static void w_aes256_enc(const uint8_t *i, uint8_t *o, size_t b, const void *c)
    { aes256_encrypt_blocks(i, o, b, (const aes256_ctx_t *)c); }
static void w_aes256_dec(const uint8_t *i, uint8_t *o, size_t b, const void *c)
    { aes256_decrypt_blocks(i, o, b, (const aes256_ctx_t *)c); }

static void w_aria128_setup(const uint8_t *k, void *c)
    { aria128_key_expand(k, (aria128_ctx_t *)c); }
static void w_aria128_enc(const uint8_t *i, uint8_t *o, size_t b, const void *c)
    { aria128_encrypt_blocks(i, o, b, (const aria128_ctx_t *)c); }
static void w_aria128_dec(const uint8_t *i, uint8_t *o, size_t b, const void *c)
    { aria128_decrypt_blocks(i, o, b, (const aria128_ctx_t *)c); }

static void w_seed_setup(const uint8_t *k, void *c)
    { seed_key_expand(k, (seed_ctx_t *)c); }
static void w_seed_enc(const uint8_t *i, uint8_t *o, size_t b, const void *c)
    { seed_encrypt_blocks(i, o, b, (const seed_ctx_t *)c); }
static void w_seed_dec(const uint8_t *i, uint8_t *o, size_t b, const void *c)
    { seed_decrypt_blocks(i, o, b, (const seed_ctx_t *)c); }

static void w_des_setup(const uint8_t *k, void *c)
    { des_key_expand(k, (des_ctx_t *)c); }
static void w_des_enc(const uint8_t *i, uint8_t *o, size_t b, const void *c)
    { des_encrypt_blocks(i, o, b, (const des_ctx_t *)c); }
static void w_des_dec(const uint8_t *i, uint8_t *o, size_t b, const void *c)
    { des_decrypt_blocks(i, o, b, (const des_ctx_t *)c); }

static void w_tdes2_setup(const uint8_t *k, void *c)
    { tdes2_key_expand(k, (tdes_ctx_t *)c); }
static void w_tdes2_enc(const uint8_t *i, uint8_t *o, size_t b, const void *c)
    { tdes2_encrypt_blocks(i, o, b, (const tdes_ctx_t *)c); }
static void w_tdes2_dec(const uint8_t *i, uint8_t *o, size_t b, const void *c)
    { tdes2_decrypt_blocks(i, o, b, (const tdes_ctx_t *)c); }

static void w_tdes3_setup(const uint8_t *k, void *c)
    { tdes3_key_expand(k, (tdes_ctx_t *)c); }
static void w_tdes3_enc(const uint8_t *i, uint8_t *o, size_t b, const void *c)
    { tdes3_encrypt_blocks(i, o, b, (const tdes_ctx_t *)c); }
static void w_tdes3_dec(const uint8_t *i, uint8_t *o, size_t b, const void *c)
    { tdes3_decrypt_blocks(i, o, b, (const tdes_ctx_t *)c); }

static aes128_ctx_t  ctx_aes128;
static aes256_ctx_t  ctx_aes256;
static aria128_ctx_t ctx_aria128;
static seed_ctx_t    ctx_seed;
static des_ctx_t     ctx_des;
static tdes_ctx_t    ctx_tdes2;
static tdes_ctx_t    ctx_tdes3;

/* Keys from NIST FIPS 197 Appendix B/C and NIST SP 800-17. */
static const uint8_t KEY_AES128[16] = {
    0x2b,0x7e,0x15,0x16, 0x28,0xae,0xd2,0xa6,
    0xab,0xf7,0x15,0x88, 0x09,0xcf,0x4f,0x3c
};
static const uint8_t KEY_AES256[32] = {
    0x60,0x3d,0xeb,0x10, 0x15,0xca,0x71,0xbe,
    0x2b,0x73,0xae,0xf0, 0x85,0x7d,0x77,0x81,
    0x1f,0x35,0x2c,0x07, 0x3b,0x61,0x08,0xd7,
    0x2d,0x98,0x10,0xa3, 0x09,0x14,0xdf,0xf4
};
static const uint8_t KEY_ARIA128[16] = {
    0x00,0x01,0x02,0x03, 0x04,0x05,0x06,0x07,
    0x08,0x09,0x0a,0x0b, 0x0c,0x0d,0x0e,0x0f
};
static const uint8_t KEY_SEED[16] = {
    0x00,0x01,0x02,0x03, 0x04,0x05,0x06,0x07,
    0x08,0x09,0x0a,0x0b, 0x0c,0x0d,0x0e,0x0f
};
static const uint8_t KEY_DES[8] = {
    0x01,0x01,0x01,0x01, 0x01,0x01,0x01,0x01
};
static const uint8_t KEY_TDES2[16] = {
    0x01,0x23,0x45,0x67, 0x89,0xab,0xcd,0xef,
    0x23,0x45,0x67,0x89, 0xab,0xcd,0xef,0x01
};
static const uint8_t KEY_TDES3[24] = {
    0x01,0x23,0x45,0x67, 0x89,0xab,0xcd,0xef,
    0x23,0x45,0x67,0x89, 0xab,0xcd,0xef,0x01,
    0x45,0x67,0x89,0xab, 0xcd,0xef,0x01,0x23
};

static bench_algo_t algos[] = {
    { "AES-128",  16, 16, w_aes128_setup,  w_aes128_enc,  w_aes128_dec,  &ctx_aes128,  KEY_AES128  },
    { "AES-256",  32, 16, w_aes256_setup,  w_aes256_enc,  w_aes256_dec,  &ctx_aes256,  KEY_AES256  },
    { "ARIA-128", 16, 16, w_aria128_setup, w_aria128_enc, w_aria128_dec, &ctx_aria128, KEY_ARIA128 },
    { "SEED",     16, 16, w_seed_setup,    w_seed_enc,    w_seed_dec,    &ctx_seed,    KEY_SEED    },
    { "DES",       8,  8, w_des_setup,     w_des_enc,     w_des_dec,     &ctx_des,     KEY_DES     },
    { "3DES-2KEY",16,  8, w_tdes2_setup,   w_tdes2_enc,   w_tdes2_dec,   &ctx_tdes2,   KEY_TDES2   },
    { "3DES-3KEY",24,  8, w_tdes3_setup,   w_tdes3_enc,   w_tdes3_dec,   &ctx_tdes3,   KEY_TDES3   },
};

#define N_ALGOS  ((int)(sizeof(algos) / sizeof(algos[0])))
#define WARMUP   3
#define REPEAT   10

int main(int argc, char *argv[])
{
    const char  *target;
    size_t       data_mb, data_bytes, aligned;
    bench_dir_t  dir;
    int          i;

    if (argc != 4) {
        fprintf(stderr,
                "Usage: %s <algo> <data_mb> <enc|dec>\n"
                "  algo   : AES-128 AES-256 ARIA-128 SEED DES 3DES-2KEY 3DES-3KEY\n"
                "  data_mb: 1 or 10\n",
                argv[0]);
        return 1;
    }

    target     = argv[1];
    data_mb    = (size_t)atoi(argv[2]);
    data_bytes = data_mb * 1024 * 1024;

    if (strcmp(argv[3], "enc") == 0)
        dir = BENCH_ENCRYPT;
    else if (strcmp(argv[3], "dec") == 0)
        dir = BENCH_DECRYPT;
    else {
        fprintf(stderr, "direction must be 'enc' or 'dec'\n");
        return 1;
    }

    for (i = 0; i < N_ALGOS; i++) {
        if (strcmp(algos[i].name, target) == 0) {
            aligned = (data_bytes / (size_t)algos[i].block_size)
                      * (size_t)algos[i].block_size;

            printf("BENCH_META algo=%s dir=%s data_bytes=%zu repeat=%d\n",
                   algos[i].name, argv[3], aligned, REPEAT);
            fflush(stdout);

            bench_run(&algos[i], aligned, WARMUP, REPEAT, dir);
            return 0;
        }
    }

    fprintf(stderr, "Unknown algorithm: %s\n", target);
    return 1;
}
