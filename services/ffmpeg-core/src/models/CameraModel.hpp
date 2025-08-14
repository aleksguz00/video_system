#include <string>

struct CameraModel
{
    std::string name;
    std::string address;
    std::string rtsp;
    std::string login;
    std::string password;
    int id;
    bool bActive;
    bool bRecordArchive;
};