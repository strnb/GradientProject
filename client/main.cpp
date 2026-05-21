#include "crypto_utils.h"

#include <iostream>
#include <string>

int main()
{
    std::string text = "Hello";

    std::string key = "secret";

    // Шифрование
    std::string encrypted =
        encrypt(text, key);

    // Расшифровка
    std::string decrypted =
        decrypt(encrypted, key);

    // Хеширование пароля
    std::string hash =
        hashPassword("12345");

    std::cout << "Original: "
              << text
              << std::endl;

    std::cout << "Encrypted: "
              << encrypted
              << std::endl;

    std::cout << "Decrypted: "
              << decrypted
              << std::endl;

    std::cout << "SHA-256: "
              << hash
              << std::endl;

    return 0;
}
