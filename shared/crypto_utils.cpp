#include "crypto_utils.h"
#include "sha256.h"

std::string hashPassword(
    const std::string& password
)
{
    return sha256(password);
}

std::string encrypt(
    const std::string& text,
    const std::string& key
)
{
    std::string result = text;

    for (size_t i = 0; i < text.size(); i++)
    {
        result[i] =
            text[i] + key[i % key.size()];
    }

    return result;
}

std::string decrypt(
    const std::string& text,
    const std::string& key
)
{
    std::string result = text;

    for (size_t i = 0; i < text.size(); i++)
    {
        result[i] =
            text[i] - key[i % key.size()];
    }

    return result;
}