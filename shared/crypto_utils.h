#ifndef SECURITY_H
#define SECURITY_H

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