#include "AmqpClient.hpp"

#include <ev.h>
#include <spdlog/spdlog.h>

AmqpClient::AmqpClient(const std::string &host, int port, const std::string &username, const std::string &password)
    : m_Host(host), m_Port(port), m_Username(username), m_Password(password)
{
}

AmqpClient::~AmqpClient()
{
    disconnect();
}

bool AmqpClient::connect()
{
    if (m_IsConnected) {
        spdlog::warn("Already connected to RabbitMQ");

        return true;
    }

    try {
        spdlog::info("Connecting to RabbitMQ at {}:{}", m_Host, m_Port);

        AMQP::Address address(m_Host, m_Port, AMQP::Login(m_Username, m_Password), "/");

        m_Handler = std::make_unique<AMQP::LibEvHandler>(m_EventLoop);
        m_Connection = std::make_unique<AMQP::TcpConnection>(m_Handler.get(), address);
        m_Channel = std::make_unique<AMQP::TcpChannel>(m_Connection.get());

        m_Channel->onError([this](const char* message) {
            spdlog::error("AMQP Channel error: {}", message);
            m_IsConnected = false;
        });

        m_IsConnected = true;
        spdlog::info("Successfully connected to RabbitMQ");

        return true;
    } catch (const std::exception& e) {
        spdlog::error("Failed to connect to RabbitMQ: {}", e.what());

        return false;
    }
}

void AmqpClient::disconnect()
{
    if (m_IsConnected) {
        try {
            if (m_Channel) {
                m_Channel->close();
            }

            if (m_Connection) {
                m_Connection->close();
            }

            m_IsConnected = false;
            spdlog::info("Disconnected from RabbitMQ");
        } catch (const std::exception& e) {
            spdlog::error("Error during RabbitMQ disconnect: {}", e.what());
        }
    }
}

bool AmqpClient::isConnected() const
{
    return m_IsConnected;
}

void AmqpClient::declareQueue(const std::string &queueName, bool durable, bool exclusive, bool autoDelete)
{
    if (!m_IsConnected) {
        spdlog::error("Not connected to RabbitMQ");

        return;
    }

    int flags = (durable ? AMQP::durable : 0) |
                (exclusive ? AMQP::exclusive : 0) |
                (autoDelete ? AMQP::autodelete : 0);

    m_Channel->declareQueue(queueName, flags)
        .onSuccess([queueName]() {
            spdlog::debug("Declared queue: {}", queueName);
        })
        .onError([queueName](const char* message) {
            spdlog::error("Failed to declare queue {}: {}", queueName, message);
        });
}

void AmqpClient::consumeMessage(const std::string &queueName, const std::function<void(const std::string&)>& callback, bool autoAck)
{
    std::string message;

    if (!m_IsConnected) {
        spdlog::error("Not connected to RabbitMQ");


    }

    m_Channel->consume(queueName, AMQP::noack)
        .onReceived([callback](const AMQP::Message& message, uint64_t deliveryTag, bool redelivered) {
            std::string body(message.body(), message.bodySize());
            callback(body);
        })
        .onError([queueName](const char* message) {
            spdlog::error("Failed to consume from queue {}: {}", queueName, message);
        });
}

void AmqpClient::publishMessage(const std::string &queueName, const std::string &message, bool persistent)
{
    if (!m_IsConnected) {
        spdlog::error("Not connected to RabbitMQ");

        return;
    }

    AMQP::Envelope env(message.c_str(), message.size());

    if (persistent) {
        env.setDeliveryMode(2);
    }

    m_Channel->publish("", queueName, env);

    spdlog::debug("Published message to queue {}: {}", queueName, message);
}

void AmqpClient::bindQueue(const std::string &queueName, const std::string &exchangeName, const std::string &routingKey)
{
    if (!m_IsConnected) {
        spdlog::error("Not connected to RabbitMQ");

        return;
    }

    m_Channel->bindQueue(exchangeName, queueName, routingKey)
        .onSuccess([=]() {
            spdlog::debug("Bound queue {} to exchange {} with key {}", queueName, exchangeName, routingKey);
        })
        .onError([=](const char* message) {
            spdlog::error("Failed to bind queue: {}", message);
        });
}
