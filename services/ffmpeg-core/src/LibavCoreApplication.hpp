#pragma once

#include "Poco/Util/ServerApplication.h"

#include <vector>
#include <iostream>

#include <jsoncpp/json/json.h>

using namespace Poco::Util;

class LibavCoreApplication : public ServerApplication
{
protected:
    int main(const std::vector<std::string>& args) override;
    void initialize(Application& self) override;

private:
    std::string getConfigPath();
};