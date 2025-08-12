#pragma once

#include <string>
#include <fstream>

#include <jsoncpp/json/json.h>

namespace Utils
{
    bool readJsonFile(const std::string& path, Json::Value& obj);
    bool validateDatabaseJson(const Json::Value& config);
}