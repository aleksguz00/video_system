#include "CamerasService.hpp"

CamerasService::CamerasService()
{
    Poco::Data::Session session = dbConnector->getSession();
    m_CameraRepository = new CameraRepository(session);
}

std::list<CameraModel> CamerasService::getAllCameras()
{
    return m_CameraRepository->getAllCameras();
}