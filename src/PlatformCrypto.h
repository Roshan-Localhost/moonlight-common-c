#pragma once

#include <stdbool.h>

#ifdef USE_MBEDTLS
#include <mbedtls/cipher.h>
#else
// Hide the real OpenSSL definition from other code
typedef struct evp_cipher_ctx_st EVP_CIPHER_CTX;
#endif

typedef struct _PLT_CRYPTO_CONTEXT {
#ifdef USE_MBEDTLS
    bool initialized;
    mbedtls_cipher_context_t ctx;
#else
    bool initialized; // Used for CBC only
    EVP_CIPHER_CTX* ctx;
#endif
} PLT_CRYPTO_CONTEXT, *PPLT_CRYPTO_CONTEXT;

#define ROUND_TO_PKCS7_PADDED_LEN(x) ((((x) + 15) / 16) * 16)

PPLT_CRYPTO_CONTEXT PltCreateCryptoContext();
void PltDestroyCryptoContext(PPLT_CRYPTO_CONTEXT ctx);

#define ALGORITHM_AES_CBC 1
#define ALGORITHM_AES_GCM 2

bool PltEncryptMessage(PPLT_CRYPTO_CONTEXT ctx, int algorithm,
                       unsigned char* key, int keyLength,
                       unsigned char* iv, int ivLength,
                       unsigned char* tag, int tagLength,
                       unsigned char* inputData, int inputDataLength,
                       unsigned char* outputData, int* outputDataLength);

bool PltDecryptMessage(PPLT_CRYPTO_CONTEXT ctx, int algorithm,
                       unsigned char* key, int keyLength,
                       unsigned char* iv, int ivLength,
                       unsigned char* tag, int tagLength,
                       unsigned char* inputData, int inputDataLength,
                       unsigned char* outputData, int* outputDataLength);

void PltGenerateRandomData(unsigned char* data, int length);