#pragma once

#include "CameraRepository.hpp"

#include "Poco/Types.h"

class CamerasService
{
public: 
    CamerasService();

    std::list<CameraModel> getAllCameras();
private:
    Poco::SharedPtr<CameraRepository> m_CameraRepository;
};