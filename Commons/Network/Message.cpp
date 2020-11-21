#include "Message.hpp"

using namespace Commons::Network;

MutableBufferArray<3> MessageHeader::getBufferSequence()
{
    return MutableBufferArray<3> {
            boost::asio::buffer(&length, sizeof(length)),
            boost::asio::buffer(&purposeByte, sizeof(purposeByte)),
            boost::asio::buffer(&taskId, sizeof(taskId))
    };
}

ConstBufferArray<3> MessageHeader::getBufferSequence() const
{
    return ConstBufferArray<3> {
            boost::asio::buffer(&length, sizeof(length)),
            boost::asio::buffer(&purposeByte, sizeof(purposeByte)),
            boost::asio::buffer(&taskId, sizeof(taskId))
    };
}

Message::Message(const MessageHeader& header, Message::ContentPointer content)
    : mHeader(header)
    , mContent(std::move(content))
{}

MessageHeader &Message::header() {
    return mHeader;
}

const MessageHeader &Message::header() const {
    return mHeader;
}

Message::ContentPointer &Message::content() {
    return mContent;
}

const Message::ContentPointer &Message::content() const {
    return mContent;
}

MutableBuffer Message::getContentBuffer() {
    return boost::asio::buffer(mContent.get(), mHeader.length);
}

ConstBuffer Message::getContentBuffer() const {
    return boost::asio::buffer(mContent.get(), mHeader.length);
}
