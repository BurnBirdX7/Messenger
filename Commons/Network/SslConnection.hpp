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
            DISCONNECTED, // SSL connection isn't established
            CONNECTED,    // SSL connection established
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

    public:
        void setReceiveListener(const ReceiveListener&);
        void setSendListener(const SendListenter&);
        void setStateListener(const StateListener&);

        // Returns current state of the connection
        State getState() const;

        // Send Message
        void send(const IMessage&);

        // Closes connection, immediately returns if mStatus == State::DISCONNECTED
        // Throws boost::system::system_error if there was an error during shutdown
        void close();

    public: // static methods
        static SslConnection makeServerSide(TcpSocket&&, SslContext&);
        static SslConnection makeClientSide(TcpSocket&&, SslContext&);

    private: // net
        void tcpConnect(const Endpoints &endpoints);
        void sslHandshake();
        void sslShutdown();

        void doReceiveHeader();
        void doReceiveBody(const MessageHeader&);

        void doSend(const ConstBuffer&);

        template <class ConstBufferSequence>
        void doSendSequence(const ConstBufferSequence&);

    private:
        void changeState(State);

        void notifyReceiveListener(const Message&);
        void notifySendListener(size_t);
        void notifyStateListener(State);

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
        if (mState == State::DISCONNECTED)
            throw Error(NetworkErrorCategory::SSL_SEND_DCONNECTED, NetworkCategory());

        boost::asio::async_write(mSocket,
                                 sequence,
                                 [this](const boost::system::error_code &ec,
                                    std::size_t bytes_transferred)
                                 {
                                     if (!ec)
                                         notifySendListener(bytes_transferred);
                                     else
                                         close(); // Log error

                                 });
    }
}

#endif //ASIOAPPLICATION_SSLCONNECTION_HPP
