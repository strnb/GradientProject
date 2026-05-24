#pragma once
#include <map>
#include <string>

class Database
{
private:
    std::map<std::string, std::string> users;

    Database() {}

public:
    static Database& instance();

    void addUser(const std::string& login, const std::string& password);
    bool exists(const std::string& login);
    std::string getUser(const std::string& login);
};