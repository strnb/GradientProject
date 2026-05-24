#include "auth_service.h"
#include "database.h"
#include "../shared/crypto_utils.h"

bool registerUser(const std::string& login, const std::string& password)
{
    auto& db = Database::instance();

    if (db.exists(login))
        return false;

    std::string hash = hashPassword(password);
    db.addUser(login, hash);

    return true;
}

bool loginUser(const std::string& login, const std::string& password)
{
    auto& db = Database::instance();

    if (!db.exists(login))
        return false;

    std::string hash = hashPassword(password);
    return db.getUser(login) == hash;
}