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

    class SslConnection {

    public: // definitions

        using TcpSocket = tcp::socket;
        using SslSocket = boost::asio::ssl::stream<TcpSocket>;
        using IoContext = boost::asio::io_context;
        using SslContext = boost::asio::ssl::context;
        using HandshakeType = boost::asio::ssl::stream_base::handshake_type;

        using ListenerFunction = std::function<void (const Message&)>;
        using ListenersContainer = std::vector<ListenerFunction>;

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

        ~SslConnection();

        void addListener(const ListenerFunction&);

        void send(const Message&);
        void send(const MessageRepresentation&);

        void closeConnection();

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

        ListenersContainer mListeners;

    };

    template <class ConstBufferSequence>
    void SslConnection::doSendSequence(const ConstBufferSequence& sequence)
    {
        boost::asio::async_write(mSocket,
                                 sequence,
                                 [](const boost::system::error_code &ec,
                                    std::size_t /* bytes_transferred */)
                                 {
                                     if (ec)
                                         throw boost::system::system_error(ec);

                                 });
    }

}

#endif //ASIOAPPLICATION_SSLCONNECTION_HPP
