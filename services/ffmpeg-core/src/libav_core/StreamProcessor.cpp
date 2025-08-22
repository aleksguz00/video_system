#include "StreamProcessor.hpp"

#include <spdlog/spdlog.h>

#include "CameraStream.hpp"

StreamProcessor::StreamProcessor() : m_ThreadPool(4, 16)
{
}

StreamProcessor::~StreamProcessor()
{
    shutdown();
}

void StreamProcessor::initialize()
{
    loadInitialCamera();


    // Реализовать конфиг кролика также, как БД

    // try {
    //     m_AmqpClient = std::make_unique<AmqpClient>("")
    // }

    m_IsRunning = true;
    m_MessageThread.start(*this);
}

void StreamProcessor::run()
{
}

void StreamProcessor::shutdown()
{
    m_IsRunning = false;

    if (m_MessageThread.isRunning()) {
        m_MessageThread.join();
    }

    Poco::ScopedLock lock(m_Mutex);

    for (auto& [id, stream] : m_Streams) {
        stream->stop();
    }

    m_Streams.clear();

    m_ThreadPool.joinAll();
}

void StreamProcessor::addCamera(const CameraModel &camera)
{
    if (!camera.bActive) {
        spdlog::warn("Camera {} is not active, skipping", camera.name);
        
        return;
    }

    if (m_Streams.find(camera.id) != m_Streams.end()) {
        spdlog::warn("Camera {} already exists, updating instead", camera.name);
        updateCamera(camera);

        return;
    }

    try {
        Poco::ScopedLock lock(m_Mutex);

        expandThreadPoolIfNeeded();
    
        auto stream = std::make_unique<CameraStream>(camera);
        CameraStream* rawPtr = stream.get();
        m_Streams.emplace(camera.id, std::move(stream));
    
        m_ThreadPool.start(*rawPtr);
    } catch (std::exception& e) {
        spdlog::error("Failed to add camera {}: {}", camera.name, e.what());
    }
}

void StreamProcessor::removeCamera(int cameraId)
{
    try {
        Poco::ScopedLock lock(m_Mutex);
    
        auto it = m_Streams.find(cameraId);
    
        if (it != m_Streams.end()) {
            it->second->stop();
    
            m_Streams.erase(it);
        }
    
        spdlog::info("Removed camera: {}", cameraId);
    } catch (std::exception& e) {
        spdlog::error("Failed to remove camera {}: {}", cameraId, e.what());
    }
}

void StreamProcessor::updateCamera(const CameraModel &camera)
{
    removeCamera(camera.id);

    if (camera.bActive) {
        addCamera(camera);
    } else {
        spdlog::info("Camera {} deactivated", camera.id);
    }
}

void StreamProcessor::loadInitialCamera()
{
    // Get request to Stream Manager;
    // Create cameras vector
    // addCamera
}

void StreamProcessor::processMessage(const CameraMessage &message)
{
    switch (message.type) {
        case CameraMessageType::ADD_CAMERA: {
            addCamera(message.camera);
            break;
        }

        case CameraMessageType::REMOVE_CAMERA: {
            removeCamera(message.camera.id);
            break;
        }

        case CameraMessageType::UPDATE_CAEMRA: {
            updateCamera(message.camera);
            break;
        }
    }
}

void StreamProcessor::expandThreadPoolIfNeeded()
{
    if (m_ThreadPool.available() == 0) {
        int newCapacity = m_ThreadPool.capacity() + 4;
        int delta = newCapacity - m_ThreadPool.capacity();

        if (delta > 0) {
            m_ThreadPool.addCapacity(delta);
            
            spdlog::info("ThreadPool expanded from {} to {}", m_ThreadPool.capacity()- delta, newCapacity);
        }
    }
}
