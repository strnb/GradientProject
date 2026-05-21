#include "../shared/crypto_utils.h"

#include <iostream>

int main()
{
    std::string password = "12345";

    std::string hash =
        hashPassword(password);

    std::cout << hash << std::endl;

    return 0;
}