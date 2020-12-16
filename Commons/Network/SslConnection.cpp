#include "SslConnection.hpp"

using namespace Commons::Network;

SslConnection::SslConnection(boost::asio::io_context& ioContext,
                             boost::asio::ssl::context& sslContext,
                             const HandshakeType& handshakeType,
                             const tcp::resolver::results_type& endpoints)
                     : mSocket(ioContext, sslContext)
                     , mHandshakeType(handshakeType)
                     , mState(State::DISCONNECTED)
{
    tcpConnect(endpoints);
}

SslConnection::SslConnection(TcpSocket&& socket,
                             SslContext& sslContext,
                             const HandshakeType& handshakeType)
    : mSocket(std::move(socket), sslContext)
    , mHandshakeType(handshakeType)
    , mState(State::DISCONNECTED)
{
    sslHandshake();
}

SslConnection::~SslConnection()
{
    close();
}

void SslConnection::setReceiveListener(const ReceiveListener& listener)
{
    mReceiveListener = listener;
}

void SslConnection::setSendListener(const SendListenter& listener)
{
    mSendListener = listener;
}


void SslConnection::setStateListener(const StateListener& listener)
{
    mStateListener = listener;
}

SslConnection::State SslConnection::getState() const
{
    return mState;
}

void SslConnection::send(const IMessage& message)
{
    doSendSequence(message.getBufferSequence());
}

void SslConnection::close()
{
    if (mState == State::DISCONNECTED)
        return;

    changeState(State::DISCONNECTED);
    sslShutdown();
}

SslConnection SslConnection::makeServerSide(SslConnection::TcpSocket&& socket, SslConnection::SslContext& context)
{
    return SslConnection(std::move(socket),
                         context,
                         boost::asio::ssl::stream_base::handshake_type::server);
}

SslConnection SslConnection::makeClientSide(SslConnection::TcpSocket&& socket, SslConnection::SslContext& context)
{
    return SslConnection(std::move(socket),
                         context,
                         boost::asio::ssl::stream_base::handshake_type::client);
}

void SslConnection::tcpConnect(const boost::asio::ip::basic_resolver_results<tcp>& endpoints)
{
    boost::asio::async_connect(mSocket.lowest_layer(),
                               endpoints,
                               [this](const boost::system::error_code &ec,
                                  const tcp::endpoint & /*endpoint*/) {
                                   if (!ec) {
                                       sslHandshake();
                                   }
                                   else
                                       changeState(State::DISCONNECTED);
                               }
    );
}

void SslConnection::sslHandshake()
{
    mSocket.async_handshake(mHandshakeType,
                            [this](const boost::system::error_code &ec) {
                                if (!ec)
                                    changeState(State::CONNECTED);
                                else
                                    close();
                            }
    );
}


void SslConnection::sslShutdown()
{
    mSocket.async_shutdown(
            [this](const boost::system::error_code &shutdown_error) {
                if (!shutdown_error)
                    mSocket.lowest_layer().close();
                else
                    throw boost::system::system_error(shutdown_error);
            }
    );
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
                                    close();
                            }
    );
}

void SslConnection::doReceiveBody(const MessageHeader &msg_header)
{
    Message message(msg_header, Message::ContentPointer(new uint8_t[msg_header.length]));
    boost::asio::async_read(mSocket,
                            message.getContentBuffer(),
                            [this, message](const boost::system::error_code &ec,
                                            std::size_t /* bytes_transferred */) {
                                if (!ec) {
                                    notifyReceiveListener(message);
                                    doReceiveHeader();
                                }
                                else {
                                    close();
                                }
                            }
    );
}

void SslConnection::doSend(const ConstBuffer& buffer)
{
    boost::asio::async_write(mSocket,
                             buffer,
                             [this](const boost::system::error_code &ec,
                                    std::size_t bytes_transferred) {
                                 if (!ec)
                                     notifySendListener(bytes_transferred);
                                 else
                                     close();
                             }
    );
}

void SslConnection::changeState(State state)
{
    if (state == mState)
        return;

    mState = state;
    std::invoke(mStateListener, state);
}

void SslConnection::notifyReceiveListener(const Message& message)
{
    if (!mReceiveListener)
        throw Error(NetworkErrorCategory::SSL_LISTENER_ISNT_SET, NetworkCategory(), "Receive listener");
    std::invoke(mReceiveListener, message);
}

void SslConnection::notifySendListener(size_t transferred)
{
    if (!mSendListener)
        throw Error(NetworkErrorCategory::SSL_LISTENER_ISNT_SET, NetworkCategory(), "Send listener");
    std::invoke(mSendListener, transferred);

}

void SslConnection::notifyStateListener(State state)
{
    //if (!mStateListener)
    //    throw Error(NetworkErrorCategory::SSL_LISTENER_ISNT_SET, NetworkCategory(), "Receive listener");
    if (mStateListener)
        std::invoke(mStateListener, state);
}

