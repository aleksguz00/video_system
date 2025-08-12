#include "CameraRepository.hpp"

#include "Poco/Data/RecordSet.h"
#include "spdlog/spdlog.h"

CameraRepository::CameraRepository(Poco::Data::Session& session) : m_Session(session)
{
    
}

std::list<CameraModel> CameraRepository::getAllCameras()
{
    std::list<CameraModel> result;
    std::string sql = "SELECT * FROM cameras";

    try
    {
        Poco::Data::RecordSet recordSet(m_Session, sql);

        for (auto more = recordSet.moveFirst(); more; more = recordSet.moveNext())
        {
            CameraModel model;
            model.id = recordSet["id"].convert<int>();
            model.address = recordSet["address"].convert<std::string>();
            model.name = recordSet["camera_name"].convert<std::string>();
            model.rtsp = recordSet["rtsp"].convert<std::string>();
            model.bActive = recordSet["is_active"].convert<bool>();
            model.bRecordArchive = recordSet["is_record_archive"].convert<bool>();

            result.push_back(model);
        }
    }
    catch (Poco::Exception& pe)
    {
        spdlog::error("Database error {}", pe.displayText());
    }

    return result;
}
