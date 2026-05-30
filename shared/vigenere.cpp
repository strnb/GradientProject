#include "vigenere.h"

#include <cctype>

std::string vigenereEncrypt(
    const std::string& text,
    const std::string& key
)
{
    std::string result;

    if (key.empty())
        return result;

    int keyIndex = 0;

    for (char c : text)
    {
        if (!std::isalpha(c))
        {
            result += c;
            continue;
        }

        char upperChar =
            std::toupper(c);

        char upperKey =
            std::toupper(
                key[keyIndex % key.size()]
            );

        int shift =
            upperKey - 'A';

        char encrypted =
            'A' +
            ((upperChar - 'A' + shift) % 26);

        result += encrypted;

        keyIndex++;
    }

    return result;
}

std::string vigenereDecrypt(
    const std::string& text,
    const std::string& key
)
{
    std::string result;

    if (key.empty())
        return result;

    int keyIndex = 0;

    for (char c : text)
    {
        if (!std::isalpha(c))
        {
            result += c;
            continue;
        }

        char upperChar =
            std::toupper(c);

        char upperKey =
            std::toupper(
                key[keyIndex % key.size()]
            );

        int shift =
            upperKey - 'A';

        char decrypted =
            'A' +
            ((upperChar - 'A' - shift + 26) % 26);

        result += decrypted;

        keyIndex++;
    }

    return result;
}