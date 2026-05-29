/* AES-128: NIST FIPS 197 Appendix B. AES-256: Appendix C.3. */

#include <stdint.h>
#include <stdio.h>
#include "test_utils.h"
#include "aes.h"

void run_aes_tests(void)
{
    FILE          *f = test_open_vectors("test/vectors/aes_ecb.txt");
    test_vector_t  v;
    uint8_t        out[16];

    while (test_load_vector(f, &v)) {
        char enc[32], dec[32];
        if (v.key_len == 16) {
            aes128_ctx_t ctx;
            snprintf(enc, sizeof(enc), "AES-128 [%d] encrypt", v.count);
            snprintf(dec, sizeof(dec), "AES-128 [%d] decrypt", v.count);
            aes128_key_expand(v.key, &ctx);
            aes128_encrypt(v.pt, out, &ctx);
            test_assert_bytes(enc, v.ct, out, 16);
            aes128_decrypt(v.ct, out, &ctx);
            test_assert_bytes(dec, v.pt, out, 16);
        } else if (v.key_len == 32) {
            aes256_ctx_t ctx;
            snprintf(enc, sizeof(enc), "AES-256 [%d] encrypt", v.count);
            snprintf(dec, sizeof(dec), "AES-256 [%d] decrypt", v.count);
            aes256_key_expand(v.key, &ctx);
            aes256_encrypt(v.pt, out, &ctx);
            test_assert_bytes(enc, v.ct, out, 16);
            aes256_decrypt(v.ct, out, &ctx);
            test_assert_bytes(dec, v.pt, out, 16);
        }
    }
    fclose(f);
}
