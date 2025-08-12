#pragma once

#include <iostream>

struct DatabaseConfig
{
    std::string hostname;
    std::string port;
    std::string dbName;
    std::string username;
    std::string password;

    int maxSessions = 10;
    int connectionRetries = 5;
    int delayMs = 1000;
};