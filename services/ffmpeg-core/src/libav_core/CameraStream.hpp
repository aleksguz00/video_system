#pragma once

#include <atomic>
#include <memory>
#include <vector>

#include <libavformat/avformat.h>
#include <Poco/Runnable.h>

#include "CameraModel.hpp"

class HLSWriter;
class ArchiveWriter;

class CameraStream : public Poco::Runnable
{
public:
    explicit CameraStream(const CameraModel& config);

    bool isRunning() const;

    void run() override;
    void stop();

    const CameraModel& getConfig() const { return m_Config; }

private:
    bool initLibav();
    void processStream();
    void clenup();

    AVFormatContext* m_InputFormatCtx;
    std::vector<AVFormatContext*> m_OutputCtxs;

    CameraModel m_Config;
    std::atomic<bool> m_IsRunning{ false };

    std::unique_ptr<HLSWriter> m_HlsWriter;
    std::unique_ptr<ArchiveWriter> m_ArchiveWriter;
};