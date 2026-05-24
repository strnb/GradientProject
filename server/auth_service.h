#pragma once
#include <string>

bool registerUser(const std::string& login, const std::string& password);
bool loginUser(const std::string& login, const std::string& password);