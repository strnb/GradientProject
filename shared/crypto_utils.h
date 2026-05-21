#ifndef CRYPTO_UTILS_H
#define CRYPTO_UTILS_H

#include <string>

std::string hashPassword(
    const std::string& password
);

std::string encrypt(
    const std::string& text,
    const std::string& key
);

std::string decrypt(
    const std::string& text,
    const std::string& key
);

#endif