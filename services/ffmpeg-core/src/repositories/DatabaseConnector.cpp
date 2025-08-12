#include "DatabaseConnector.hpp"

#include "spdlog/spdlog.h"

#include "utils/Utils.hpp"

Poco::SharedPtr<DatabaseConnector> dbConnector;

Poco::Data::Session DatabaseConnector::getSession()
{
    auto session = m_SessionPool->get();

    if (!session.isConnected())
    {
        session.reconnect();
    }

    if (!session.isConnected()) // Поправить на количество, а также добавить флаг, что соединение установлено
    {
        spdlog::error("Couldn't connect to DB session");
    }

    return session;
}

void DatabaseConnector::setConnectionParams(const DatabaseConfig &dbConfig)
{
    m_Hostname = dbConfig.hostname;
    m_Port = dbConfig.port;
    m_DbName = dbConfig.dbName;
    m_Username = dbConfig.username;
    m_Password = dbConfig.password;

    m_MaxConnectionRetries = dbConfig.connectionRetries;
    m_ReconnectionDelayMs = dbConfig.delayMs;

    m_SessionPool = new Poco::Data::SessionPool(M_CONNECTOR, getConnectionString(m_Hostname, m_Port, m_DbName, m_Username, m_Password), m_MaxSessions);
}

std::string DatabaseConnector::getConnectionString(const std::string &hostname, const std::string &port, const std::string &dbName, const std::string &username, const std::string &password)
{
    std::string result = 
            std::string("host=") + hostname + std::string("port=") + port + std::string("dbname=") + dbName + std::string("user=") + username + std::string("password") + password;

    return result;
}
