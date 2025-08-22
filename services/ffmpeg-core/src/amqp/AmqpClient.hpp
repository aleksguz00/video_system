#pragma once

#include <atomic>
#include <string>

#include <amqpcpp.h>
#include <amqpcpp/libev.h>

class AmqpClient
{
public:
    AmqpClient(const std::string& host, int port, const std::string& username, const std::string& password);
    AmqpClient(const AmqpClient&) = delete;
    AmqpClient& operator=(const AmqpClient&) = delete;
    ~AmqpClient();

    bool connect();
    void disconnect();
    bool isConnected() const;

    void declareQueue(const std::string& queueName, bool durable = false, bool exclusive = false, bool autoDelete = false);
    void consumeMessage(const std::string& queueName, const std::function<void(const std::string&)>& callback, bool autoAck = false);
    void publishMessage(const std::string& queueName, const std::string& message, bool persistent = false);
    void bindQueue(const std::string& queueName, const std::string& exchangeName, const std::string& routingKey = "");

private:
    std::string m_Host;
    int m_Port;
    std::string m_Username;
    std::string m_Password;

    std::atomic<bool> m_IsConnected{ false };

    std::unique_ptr<AMQP::TcpConnection> m_Connection;
    std::unique_ptr<AMQP::TcpChannel> m_Channel;
    std::unique_ptr<AMQP::LibEvHandler> m_Handler;

    struct ev_loop* m_EventLoop;
};