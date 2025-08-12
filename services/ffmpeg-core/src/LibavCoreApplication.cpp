#include "LibavCoreApplication.hpp"

#include "spdlog/spdlog.h"

#include "repositories/DatabaseConnector.hpp"

int LibavCoreApplication::main(const std::vector<std::string> &args)
{

    return Application::EXIT_OK;
}

void LibavCoreApplication::initialize(Application &self)
{
    DatabaseConfig dbConfig;

    loadConfiguration(getConfigPath());
    Application::initialize(self);

    std::string databaseConfigPath = config().getString("database.hostname");

    spdlog::info("Path - {}", databaseConfigPath);

    dbConfig.hostname = config().getString("database.hostname");
    dbConfig.port = config().getString("database.port");
    dbConfig.dbName = config().getString("database.db_name");
    dbConfig.username = config().getString("database.username");
    dbConfig.password = config().getString("database.password");
    dbConfig.maxSessions = config().getInt("database.max_sessions");
    dbConfig.connectionRetries = config().getInt("database.connection_retries");
    dbConfig.delayMs = config().getInt("database.delay_ms");

    dbConnector = new DatabaseConnector();
    dbConnector->setConnectionParams(dbConfig);
}

std::string LibavCoreApplication::getConfigPath()
{
    Poco::Path appPath = Poco::Util::Application::instance().commandPath();
    appPath.parent();
    appPath.setFileName("application.ini");

    return appPath.toString();
}

POCO_SERVER_MAIN(LibavCoreApplication);