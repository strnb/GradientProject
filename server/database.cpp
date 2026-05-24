#include "database.h"

Database& Database::instance()
{
    static Database instance;
    return instance;
}

void Database::addUser(const std::string& login, const std::string& password)
{
    users[login] = password;
}

bool Database::exists(const std::string& login)
{
    return users.find(login) != users.end();
}

std::string Database::getUser(const std::string& login)
{
    return users[login];
}