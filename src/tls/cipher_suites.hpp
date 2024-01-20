#pragma once

#include <sys/types.h>
#include <vector>

namespace krkr {

    namespace tls {
        // TLS 1.0 - 1.2 cipher suites.
        constexpr u_int16_t TLS_RSA_WITH_RC4_128_SHA                = 0x0005;
        constexpr u_int16_t TLS_RSA_WITH_3DES_EDE_CBC_SHA           = 0x000a;
        constexpr u_int16_t TLS_RSA_WITH_AES_128_CBC_SHA            = 0x002f;
        constexpr u_int16_t TLS_RSA_WITH_AES_256_CBC_SHA            = 0x0035;
        constexpr u_int16_t TLS_RSA_WITH_AES_128_CBC_SHA256         = 0x003c;
        constexpr u_int16_t TLS_RSA_WITH_AES_256_CBC_SHA256         = 0x003d;
        constexpr u_int16_t TLS_RSA_WITH_AES_128_GCM_SHA256         = 0x009c;
        constexpr u_int16_t TLS_RSA_WITH_AES_256_GCM_SHA384         = 0x009d;
        constexpr u_int16_t TLS_ECDHE_ECDSA_WITH_RC4_128_SHA        = 0xc007;
        constexpr u_int16_t TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA    = 0xc009;
        constexpr u_int16_t TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA    = 0xc00a;
        constexpr u_int16_t TLS_ECDHE_RSA_WITH_RC4_128_SHA          = 0xc011;
        constexpr u_int16_t TLS_ECDHE_RSA_WITH_3DES_EDE_CBC_SHA     = 0xc012;
        constexpr u_int16_t TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA      = 0xc013;
        constexpr u_int16_t TLS_ECDHE_RSA_WITH_AES_256_CBC_SHA      = 0xc014;
        constexpr u_int16_t TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256 = 0xc023;
        constexpr u_int16_t TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256   = 0xc027;
        constexpr u_int16_t TLS_ECDHE_RSA_WITH_AES_128_GCM_SHA256   = 0xc02f;
        constexpr u_int16_t TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 = 0xc02b;
        constexpr u_int16_t TLS_ECDHE_RSA_WITH_AES_256_GCM_SHA384   = 0xc030;
        constexpr u_int16_t TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384 = 0xc02c;
        constexpr u_int16_t TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305    = 0xcca8;
        constexpr u_int16_t TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305  = 0xcca9;

        // TLS 1.3 cipher suites.
        constexpr u_int16_t TLS_AES_128_GCM_SHA256                  = 0x1301;
        constexpr u_int16_t TLS_AES_256_GCM_SHA384                  = 0x1302;
        constexpr u_int16_t TLS_CHACHA20_POLY1305_SHA256            = 0x1303;

        constexpr u_int16_t TLS_FALLBACK_SCSV                       = 0x5600;

        constexpr u_int16_t TLS_ECDHE_ECDSA_WITH_3DES_EDE_CBC_SHA   = 0xc008;

        constexpr u_int16_t TLS_ECDHE_ECDSA_WITH_CHACHA20_POLY1305_SHA256 = 0xcca9;
        constexpr u_int16_t TLS_ECDHE_RSA_WITH_CHACHA20_POLY1305_SHA256   = 0xccab;

        struct client_hello_spec {
            std::vector<u_int16_t> cipher_suites;
            std::vector<u_int8_t>  compression_methods;

        };

    } // namespace tls


} // namespace krkr
