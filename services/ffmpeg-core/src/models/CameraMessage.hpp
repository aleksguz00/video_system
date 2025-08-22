#pragma once

#include <string>

#include "CameraModel.hpp"

enum class CameraMessageType
{
    ADD_CAMERA,
    REMOVE_CAMERA,
    UPDATE_CAEMRA
};

struct CameraMessage
{
    CameraMessageType type;
    CameraModel camera;

    static CameraMessage deserialize(const std::string& data);
    std::string serialize() const;
};