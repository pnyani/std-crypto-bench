/* DES: NIST SP 800-17 Table 1 KAT. 3DES: K1=K2=K3 degenerate check. */

#include <stdint.h>
#include <stdio.h>
#include "test_utils.h"
#include "des.h"
#include "3des.h"

void run_des_tests(void)
{
    FILE          *f = test_open_vectors("test/vectors/des_ecb.txt");
    test_vector_t  v;
    uint8_t        out[8];

    while (test_load_vector(f, &v)) {
        des_ctx_t ctx;
        char      enc[32], dec[32];
        snprintf(enc, sizeof(enc), "DES [%d] encrypt", v.count);
        snprintf(dec, sizeof(dec), "DES [%d] decrypt", v.count);
        des_key_expand(v.key, &ctx);
        des_encrypt(v.pt, out, &ctx);
        test_assert_bytes(enc, v.ct, out, 8);
        des_decrypt(v.ct, out, &ctx);
        test_assert_bytes(dec, v.pt, out, 8);

        /* E_K3(D_K2(E_K1(P))) with K1=K2=K3=K reduces to E_K(P). */
        {
            tdes_ctx_t tdes_ctx;
            uint8_t    tdes_key[24], des_out[8], tdes_out[8];
            char       tenc[48], tdec[48];
            int        i;

            for (i = 0; i < 8; i++) {
                tdes_key[i]    = v.key[i];
                tdes_key[i+8]  = v.key[i];
                tdes_key[i+16] = v.key[i];
            }

            snprintf(tenc, sizeof(tenc), "3DES K1=K2=K3 == DES [%d] (encrypt)", v.count);
            snprintf(tdec, sizeof(tdec), "3DES K1=K2=K3 == DES [%d] (decrypt)", v.count);

            des_key_expand(v.key, &ctx);
            des_encrypt(v.pt, des_out, &ctx);

            tdes3_key_expand(tdes_key, &tdes_ctx);
            tdes3_encrypt(v.pt, tdes_out, &tdes_ctx);

            test_assert_bytes(tenc, des_out, tdes_out, 8);

            des_decrypt(des_out, des_out, &ctx);
            tdes3_decrypt(tdes_out, tdes_out, &tdes_ctx);

            test_assert_bytes(tdec, v.pt, tdes_out, 8);
        }
    }
    fclose(f);
}
