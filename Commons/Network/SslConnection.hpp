#ifndef ASIOAPPLICATION_SSLCONNECTION_HPP
#define ASIOAPPLICATION_SSLCONNECTION_HPP

#include <cstdint>
#include <memory>
#include <optional>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "Task.hpp"
#include "TaskManager.hpp"

#include "Purpose.hpp"
#include "Message.hpp"

namespace Commons::Network {

    /*
     * SslConnection does NOT guarantee no interleaving.
     * Use SentListener to organize write
     */
    class SslConnection {

    public: // definitions

        using TcpSocket = tcp::socket;
        using SslSocket = boost::asio::ssl::stream<TcpSocket>;
        using IoContext = boost::asio::io_context;
        using SslContext = boost::asio::ssl::context;
        using HandshakeType = boost::asio::ssl::stream_base::handshake_type;

        using ReceiveListener = std::function<void (const Message&)>;
        using ReceiveListenersContainer = std::vector<ReceiveListener>;
        using SendListenter = std::function<void (size_t)>;

    public: // methods

        // Creates new TCP connection
        SslConnection(
                IoContext& ioContext,
                SslContext& sslContext,
                const HandshakeType&,
                const tcp::resolver::results_type& endpoints
                );

        // Runs over existing TCP connection
        SslConnection(
                TcpSocket&& socket,
                SslContext& sslContext,
                const HandshakeType&
                );

        SslConnection(SslConnection&&) = default;

        ~SslConnection();

        void addReceiveListener(const ReceiveListener&);
        void setSendListener(const SendListenter &listener);

        void send(const Message&);
        void send(const MessageRepresentation&);

        void closeConnection();

    public: // static methods

        static SslConnection makeServerSide(TcpSocket&&, SslContext&);
        static SslConnection makeClientSide(TcpSocket&&, SslContext&);

    private: // methods

        void doConnect(const tcp::resolver::results_type&);
        void doHandshake();

        void doReceive();
        void doReceiveHeader();
        void doReceiveBody(const MessageHeader &);

        template <class ConstBufferSequence>
        void doSendSequence(const ConstBufferSequence&);

        void doSend(const ConstBuffer&);

        void notifyListeners(const Message&);

    private: // fields

        SslSocket mSocket;
        HandshakeType mHandshakeType;
        SendListenter mSendListener;
        ReceiveListenersContainer mListeners;

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
                                         throw boost::system::system_error(ec);

                                 });
    }
}

#endif //ASIOAPPLICATION_SSLCONNECTION_HPP
