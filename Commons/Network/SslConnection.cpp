#include "SslConnection.hpp"

using namespace Commons::Network;

SslConnection::SslConnection(boost::asio::io_context& ioContext,
                             boost::asio::ssl::context& sslContext,
                             const HandshakeType& handshakeType,
                             const tcp::resolver::results_type& endpoints)
                     : mSocket(ioContext, sslContext)
                     , mHandshakeType(handshakeType)
                     , mState(State::IDLE)
{
    connect(endpoints);
}

SslConnection::SslConnection(TcpSocket&& socket,
                             SslContext& sslContext,
                             const HandshakeType& handshakeType)
    : mSocket(std::move(socket), sslContext)
    , mHandshakeType(handshakeType)
    , mState(State::TCP_IDLE)
{}

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


void SslConnection::start()
{
    if (!mSendListener || !mReceiveListener || !mStateListener)
        throw Error(NetworkCategory::SSL_LISTENERS, NetworkCategory(), "Not all listeners were set");

    changeState(State::RUNNING);
    boost::asio::post(
            [this] {
                handshake();
            }
    );
}

void SslConnection::send(const IMessage& message)
{
    doSendSequence(message.getBufferSequence());
}

void SslConnection::close()
{
    auto state = mState;
    changeState(State::CLOSING);
    switch (state)
    {
        case State::CLOSED:
        case State::CLOSING:
        case State::IDLE:
            break;
        case State::TCP_IDLE:
            mSocket.lowest_layer().close();
            changeState(State::CLOSED);
            break;
        case State::RUNNING:
        case State::SSL_IDLE:
            mSocket.lowest_layer().cancel();
            sslShutdown();
            break;
    }

    else if (mState ==)

    mSocket.lowest_layer().cancel();


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

void SslConnection::connect(const boost::asio::ip::basic_resolver_results<tcp> &endpoints)
{
    boost::asio::async_connect(mSocket.lowest_layer(),
                               endpoints,
                               [this](const boost::system::error_code &ec,
                                  const tcp::endpoint & /*endpoint*/) {
                                   if (!ec)
                                       changeState(State::TCP_IDLE);
                                   else
                                       throw boost::system::system_error(ec);
                               }
    );
}

void SslConnection::handshake()
{
    mSocket.async_handshake(mHandshakeType,
                            [this](const boost::system::error_code &ec) {
                                if (!ec)
                                    changeState(State::SSL_IDLE);
                                else
                                    close();
                            }
    );
}


void SslConnection::sslShutdown()
{
    if (mState == State::RUNNING)
        throw Error(NetworkCategory::SSL_SD_RUN_CONN, NetworkCategory(), "Attempt of"
                                                                         "shutting SSL of running connection down");

    auto self = shared_from_this();
    mSocket.async_shutdown(
            [self](const boost::system::error_code &shutdown_error) {
                if (!shutdown_error) {
                    self->mSocket.lowest_layer().close();
                }
                else {
                    throw boost::system::system_error(shutdown_error);
                }
                self->changeState(State::CLOSED);
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
                                    std::invoke(mReceiveListener, message);
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
                                     std::invoke(mSendListener, bytes_transferred);
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
