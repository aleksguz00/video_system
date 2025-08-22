#pragma once

#include <atomic>
#include <map>

#include <Poco/Mutex.h>
#include <Poco/Runnable.h>
#include <Poco/Thread.h>
#include <Poco/ThreadPool.h>

#include "CameraMessage.hpp"
#include "CameraModel.hpp"

class AmqpClient;
class CameraStream;

class StreamProcessor : public Poco::Runnable
{
public:
    StreamProcessor();
    ~StreamProcessor();

    void initialize();
    void run() override;
    void shutdown();

private:

    mutable Poco::FastMutex m_Mutex;
    Poco::ThreadPool m_ThreadPool;
    Poco::Thread m_MessageThread;
    std::atomic<bool> m_IsRunning{ false };

    std::map<int, std::unique_ptr<CameraStream>> m_Streams;

    std::unique_ptr<AmqpClient> m_AmqpClient;

    void addCamera(const CameraModel& camera);
    void removeCamera(int cameraId);
    void updateCamera(const CameraModel& camera);

    void loadInitialCamera();
    void processMessage(const CameraMessage& message);

    void expandThreadPoolIfNeeded();

};