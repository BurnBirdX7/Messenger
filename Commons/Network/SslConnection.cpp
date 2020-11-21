#include "SslConnection.hpp"

using namespace Commons::Network;

SslConnection::SslConnection(boost::asio::io_context& ioContext,
                             boost::asio::ssl::context& sslContext,
                             const HandshakeType& handshakeType,
                             const tcp::resolver::results_type& endpoints)
                     : mSocket(ioContext, sslContext)
                     , mListeners(0)
                     , mHandshakeType(handshakeType)
{
    doConnect(endpoints);
}

void SslConnection::doConnect(const tcp::resolver::results_type& endpoints)
{
    boost::asio::async_connect(mSocket.lowest_layer(),
                               endpoints,
                               [this](const boost::system::error_code &ec,
                                             const tcp::endpoint& /*endpoint*/)
                               {
                                   if (!ec)
                                       doHandshake();
                                   else
                                       throw boost::system::system_error(ec);
                               });
}

SslConnection::SslConnection(TcpSocket&& socket,
                             SslContext& sslContext,
                             const HandshakeType& handshakeType)
    : mSocket(std::move(socket), sslContext)
    , mListeners(0)
    , mHandshakeType(handshakeType)
{
    doHandshake();
}

SslConnection::~SslConnection() {
    closeConnection();
}


void SslConnection::doHandshake()
{
    mSocket.async_handshake(mHandshakeType,
                            [this](const boost::system::error_code &ec) {
                                if (!ec)
                                    doReceive();
                                else
                                    throw boost::system::system_error(ec);
                            });
}

inline void SslConnection::doReceive()
{
    doReceiveHeader();
}

void SslConnection::doReceiveHeader() {

    auto msg_header = std::make_shared<MessageHeader>();

    boost::asio::async_read(mSocket,
                            msg_header->getBufferSequence(),
                            [this, msg_header](const boost::system::error_code &ec,
                                                      std::size_t /*bytes_received*/)
                            {
                                if (!ec)
                                    doReceiveBody(*msg_header);
                                else
                                    throw boost::system::system_error(
                                            ec); // TODO: replace exceptions with error resolve
                            });
}

void SslConnection::doReceiveBody(const MessageHeader &msg_header)
{
    Message message(msg_header, Message::ContentPointer(new uint8_t[msg_header.length]));

    boost::asio::async_read(mSocket,
                            message.getContentBuffer(),
                            [this, message](const boost::system::error_code &ec,
                                                   std::size_t /* bytes_transferred */)
                            {
                                if (!ec)
                                    notifyListeners(message);
                                else
                                    throw boost::system::system_error(ec); // TODO: replace exception
                            });
}

void SslConnection::addReceiveListener(const ReceiveListener& function)
{
    mListeners.push_back(function);
}

void SslConnection::notifyListeners(const Message& message)
{
    for (auto & mListener : mListeners)
        std::invoke(mListener, message);
}

void SslConnection::doSend(const ConstBuffer& buffer)
{
    boost::asio::async_write(mSocket,
                             buffer,
                             [this](const boost::system::error_code &ec,
                                std::size_t bytes_transferred)
                             {
                                 if (!ec)
                                     std::invoke(mSendListener, bytes_transferred);
                                 else
                                     throw boost::system::system_error(ec);

                             });
}

void SslConnection::send(const Message& message)
{
    doSendSequence(message.header().getBufferSequence());
    doSendSequence(message.getContentBuffer());
}

void SslConnection::send(const MessageRepresentation& message)
{
    doSendSequence(message.getBufferSequence());
}

void SslConnection::closeConnection()
{
    mSocket.lowest_layer().cancel();
    mSocket.async_shutdown(
            [this](const boost::system::error_code& ec) {
                if (!ec)
                    mSocket.lowest_layer().close();
                else
                    throw boost::system::system_error(ec); // TODO: Exception
            });
}

void SslConnection::setSendListener(const SendListenter& listener)
{
    mSendListener = listener;
}
