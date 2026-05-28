#include "crypto_utils.h"
#include "sha256.h"

std::string hashPassword(
    const std::string& password
)
{
    return sha256(password);
}