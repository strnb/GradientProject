#include "request_handler.h"
#include "auth_service.h"
#include <sstream>

std::string handleRequest(const std::string& request)
{
    std::istringstream iss(request);
    std::string command, login, password;

    iss >> command >> login >> password;

    if (command == "REGISTER")
    {
        if (registerUser(login, password))
            return "REGISTER_SUCCESS";
        else
            return "REGISTER_FAIL";
    }

    if (command == "LOGIN")
    {
        if (loginUser(login, password))
            return "LOGIN_SUCCESS";
        else
            return "LOGIN_FAIL";
    }

    if (command == "PING")
    {
        return "PONG";
    }

    return "UNKNOWN_COMMAND";
}