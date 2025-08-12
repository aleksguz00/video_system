#include "Utils.hpp"

#include "spdlog/spdlog.h"

bool Utils::readJsonFile(const std::string &path, Json::Value &obj)
{
    std::ifstream file(path);

    if (!file.is_open())
    {
        spdlog::error("Unable to open json file {}", path);

        return false;
    }

    Json::CharReaderBuilder builder;
    Json::parseFromStream(builder, file, &obj, nullptr);

    if (file.fail())
    {
        spdlog::error("Unable to parse json file {}", path);

        return false;
    }

    return true;
}

bool Utils::validateDatabaseJson(const Json::Value &config)
{
    if (!config.isMember("hostname") || !config["hostname"].isString() || config["hostname"].asString().empty())
    {
        spdlog::error("Invalid database hostname");

        return false;
    }

    if (!config.isMember("port") || !config["port"].isString() || config["port"].asString().empty())
    {
        spdlog::error("Invalid database port");

        return false;
    }

    if (!config.isMember("db_name") || !config["db_name"].isString() || config["db_name"].asString().empty())
    {
        spdlog::error("Invalid database name");

        return false;
    }

    if (!config.isMember("username") || !config["username"].isString() || config["username"].asString().empty())
    {
        spdlog::error("Invalid database username");

        return false;
    }

    if (!config.isMember("password") || !config["password"].isString() || config["password"].asString().empty())
    {
        spdlog::error("Invalid database password");

        return false;
    }

    if (!config.isMember("max_sessions"))
    {
        spdlog::warn("Max sessions count wasn't specify. App using default value now");
    }

    if (!config.isMember("connection_retries"))
    {
        spdlog::warn("Reconnection retries count wasn't specify. App using default value now");
    }

    if (!config.isMember("delay_ms"))
    {
        spdlog::warn("Reconnection delay time wasn't specify. App using default value now");
    }

    return true;
}
