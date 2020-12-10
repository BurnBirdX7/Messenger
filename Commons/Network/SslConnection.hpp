#ifndef ASIOAPPLICATION_SSLCONNECTION_HPP
#define ASIOAPPLICATION_SSLCONNECTION_HPP

// STD
#include <cstdint>
#include <memory>
#include <optional>

// Boost
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

// Commons::Network Error Category
#include "NetworkErrorCategory.hpp"

// local
#include "Task.hpp"
#include "TaskManager.hpp"
#include "Purpose.hpp"
#include "Message.hpp"


namespace Commons::Network {

    /*
     * Asynchronously gets messages from connection and allows to asynchronously send them.
     *
     * Does NOT guarantee no interleaving.
     * Use SentListener to organize write
     */
    class SslConnection
        : public std::enable_shared_from_this<SslConnection>
    {
    public: // definitions
        enum class State : uint8_t {
            CLOSED,   // Connection was closed
            CLOSING,  // Connection are closing
            IDLE,     // Connection isn't established, default state
            TCP_IDLE, // TCP connection established
            SSL_IDLE, // SSL connection established | handshake successfully finished
            RUNNING , // Connection is running
        };

        using TcpSocket = tcp::socket;
        using SslSocket = boost::asio::ssl::stream<TcpSocket>;
        using IoContext = boost::asio::io_context;
        using SslContext = boost::asio::ssl::context;
        using Endpoints = tcp::resolver::results_type;
        using HandshakeType = boost::asio::ssl::stream_base::handshake_type;

        using ReceiveListener = std::function<void (const Message&)>;
        using SendListenter   = std::function<void (size_t)>;
        using StateListener   = std::function<void (State)>;

    public:
        // Creates new TCP connection
        SslConnection(
                IoContext& ioContext,
                SslContext& sslContext,
                const HandshakeType&,
                const Endpoints& endpoints
                );

        // Runs over existing TCP connection
        SslConnection(
                TcpSocket&& socket,
                SslContext& sslContext,
                const HandshakeType&
                );

        SslConnection(SslConnection&&) = default;

        ~SslConnection();

        void setReceiveListener(const ReceiveListener&);
        void setSendListener(const SendListenter&);
        void setStateListener(const StateListener&);

        // Returns current state of the connection
        State getState() const;

        // Executes handshake and starts receive data
        void start();

        // Send Message
        void send(const IMessage&);

        // Closes connection, immediately returns if mStatus == State::CLOSED
        // Throws boost::system::system_error if there was an error during shutdown
        void close();

    public: // static methods
        static SslConnection makeServerSide(TcpSocket&&, SslContext&);
        static SslConnection makeClientSide(TcpSocket&&, SslContext&);

    private: // net
        void connect(const Endpoints &endpoints);
        void handshake();
        void sslShutdown();

        void doReceiveHeader();
        void doReceiveBody(const MessageHeader&);

        void doSend(const ConstBuffer&);

        template <class ConstBufferSequence>
        void doSendSequence(const ConstBufferSequence&);

    private:
        void changeState(State);

    private:
        SslSocket mSocket;
        HandshakeType mHandshakeType;

        SendListenter mSendListener;
        ReceiveListener mReceiveListener;
        StateListener mStateListener;

        State mState;
    };

    template <class ConstBufferSequence>
    void SslConnection::doSendSequence(const ConstBufferSequence& sequence)
    {
        boost::asio::async_write(mSocket,
                                 sequence,
                                 [this](const boost::system::error_code &ec,
                                    std::size_t bytes_transferred)
                                 {
                                     if (!ec)
                                         std::invoke(mSendListener, bytes_transferred);
                                     else
                                         close(); // Log error

                                 });
    }
}

#endif //ASIOAPPLICATION_SSLCONNECTION_HPP
