#include "sha256.h"

#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdint>

namespace
{
    uint32_t rotateRight(uint32_t value, uint32_t bits)
    {
        return (value >> bits) | (value << (32 - bits));
    }
}

std::string sha256(const std::string& input)
{
    static const uint32_t k[64] =
    {
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
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    uint32_t h[8] =
    {
        0x6a09e667,
        0xbb67ae85,
        0x3c6ef372,
        0xa54ff53a,
        0x510e527f,
        0x9b05688c,
        0x1f83d9ab,
        0x5be0cd19
    };

    std::vector<uint8_t> data(input.begin(), input.end());

    uint64_t bitLength = data.size() * 8;

    // Добавляем 1 бит
    data.push_back(0x80);

    // Дополняем нулями до 448 бит
    while ((data.size() * 8) % 512 != 448)
    {
        data.push_back(0x00);
    }

    // Добавляем длину сообщения
    for (int i = 7; i >= 0; --i)
    {
        data.push_back(
            (bitLength >> (i * 8)) & 0xff
        );
    }

    // Обрабатываем блоки по 512 бит
    for (size_t chunk = 0; chunk < data.size(); chunk += 64)
    {
        uint32_t w[64];

        // Первые 16 слов
        for (int i = 0; i < 16; ++i)
        {
            w[i] =
                (data[chunk + i * 4] << 24) |
                (data[chunk + i * 4 + 1] << 16) |
                (data[chunk + i * 4 + 2] << 8) |
                (data[chunk + i * 4 + 3]);
        }

        // Остальные 48 слов
        for (int i = 16; i < 64; ++i)
        {
            uint32_t s0 =
                rotateRight(w[i - 15], 7) ^
                rotateRight(w[i - 15], 18) ^
                (w[i - 15] >> 3);

            uint32_t s1 =
                rotateRight(w[i - 2], 17) ^
                rotateRight(w[i - 2], 19) ^
                (w[i - 2] >> 10);

            w[i] =
                w[i - 16] +
                s0 +
                w[i - 7] +
                s1;
        }

        uint32_t a = h[0];
        uint32_t b = h[1];
        uint32_t c = h[2];
        uint32_t d = h[3];
        uint32_t e = h[4];
        uint32_t f = h[5];
        uint32_t g = h[6];
        uint32_t hh = h[7];

        // Основной цикл
        for (int i = 0; i < 64; ++i)
        {
            uint32_t S1 =
                rotateRight(e, 6) ^
                rotateRight(e, 11) ^
                rotateRight(e, 25);

            uint32_t ch =
                (e & f) ^
                ((~e) & g);

            uint32_t temp1 =
                hh + S1 + ch + k[i] + w[i];

            uint32_t S0 =
                rotateRight(a, 2) ^
                rotateRight(a, 13) ^
                rotateRight(a, 22);

            uint32_t maj =
                (a & b) ^
                (a & c) ^
                (b & c);

            uint32_t temp2 =
                S0 + maj;

            hh = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        // Добавляем результат к текущему хешу
        h[0] += a;
        h[1] += b;
        h[2] += c;
        h[3] += d;
        h[4] += e;
        h[5] += f;
        h[6] += g;
        h[7] += hh;
    }

    // Преобразуем в hex строку
    std::stringstream ss;

    for (int i = 0; i < 8; ++i)
    {
        ss << std::hex
           << std::setw(8)
           << std::setfill('0')
           << h[i];
    }

    return ss.str();
}