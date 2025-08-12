#pragma once

#include <list>

#include "Poco/Data/Session.h"

#include "CameraModel.hpp"
#include "DatabaseConnector.hpp"

class CameraRepository
{
public:
    CameraRepository(Poco::Data::Session& session);

    std::list<CameraModel> getAllCameras();
private:
    Poco::Data::Session m_Session;
};