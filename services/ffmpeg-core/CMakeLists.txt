cmake_minimum_required(VERSION 3.14)
project(ffmpeg_core)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

configure_file(
    ${CMAKE_SOURCE_DIR}/resources/application.ini
    ${CMAKE_BINARY_DIR}/application.ini
    COPYONLY
)

find_package(Poco REQUIRED COMPONENTS Foundation Net Util Data)
find_package(jsoncpp REQUIRED)

include(FindPkgConfig)
pkg_check_modules(AVFORMAT REQUIRED libavformat)
pkg_check_modules(AVCODEC REQUIRED libavcodec)
pkg_check_modules(AVUTIL REQUIRED libavutil)
pkg_check_modules(AVSWSCALE REQUIRED libswscale)
pkg_check_modules(AVFILTER REQUIRED libavfilter)

set(SOURCES
    src/LibavCoreApplication.cpp

    src/repositories/DatabaseConnector.cpp

    src/utils/Utils.cpp
)

# Создать исполняемый файл
add_executable(ffmpeg-core ${SOURCES})

# Подключить библиотеки
target_link_libraries(ffmpeg-core
    ${AVFORMAT_LIBRARIES}
    ${AVCODEC_LIBRARIES}
    ${AVUTIL_LIBRARIES}
    ${AVSWSCALE_LIBRARIES}
    ${AVDEVICE_LIBRARIES}
    ${AVFILTER_LIBRARIES}
    Poco::Foundation
    Poco::Net
    Poco::Util
    Poco::Data
    jsoncpp
)

# Добавить include-директории
target_include_directories(ffmpeg-core PRIVATE include)
target_include_directories(ffmpeg-core PRIVATE src)
target_include_directories(ffmpeg-core SYSTEM PRIVATE
    ${AVFORMAT_INCLUDE_DIRS}
    ${AVCODEC_INCLUDE_DIRS}
    ${AVUTIL_INCLUDE_DIRS}
    ${AVSWSCALE_INCLUDE_DIRS}
    ${AVDEVICE_INCLUDE_DIRS}
    ${AVFILTER_INCLUDE_DIRS}
    ${jsoncpp_INCLUDE_DIR}
)