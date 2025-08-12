#pragma once

#include <string>

#include "Poco/Data/PostgreSQL/Connector.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/SessionPool.h"

#include "utils/DatabaseConfig.hpp"

class DatabaseConnector
{
public:
    DatabaseConnector() = default;
    
    Poco::Data::Session getSession();
    void setConnectionParams(const DatabaseConfig& dbConfig);

private:
    Poco::SharedPtr<Poco::Data::SessionPool> m_SessionPool;

    const std::string M_CONNECTOR = "PostgreSQL";
    
    std::string m_Hostname;
    std::string m_Port;
    std::string m_DbName;
    std::string m_Username;
    std::string m_Password;

    int m_MaxSessions;
    int m_MaxConnectionRetries;
    int m_ReconnectionDelayMs;

    std::string getConnectionString(const std::string& hostname, const std::string& port, const std::string& dbName, const std::string& username, const std::string& password);
};

extern Poco::SharedPtr<DatabaseConnector> dbConnector;