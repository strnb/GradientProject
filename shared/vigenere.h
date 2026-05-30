#ifndef VIGENERE_H
#define VIGENERE_H

#include <string>

std::string vigenereEncrypt(
    const std::string& text,
    const std::string& key
);

std::string vigenereDecrypt(
    const std::string& text,
    const std::string& key
);

#endif