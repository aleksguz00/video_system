#pragma once

#include "DatabaseConnector.hpp"

#include "Poco/Data/Session.h"

class CameraRepository
{
public:
    CameraRepository(Poco::Data::Session& session);

private:
    Poco::Data::Session m_Session;
};