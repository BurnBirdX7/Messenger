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

Message::Message(const MessageHeader& header, Message::ContentPointer&& content)
    : mHeader(header)
    , mContent(std::move(content))
{}

Message::Message(const MessageHeader& header, Message::RawContentPointer pointer, size_t size)
    : mHeader(header)
    , mContent(new uint8_t[size])
{
    assert (header.length == size);

    for (ptrdiff_t i = 0; i < size; ++i)
        mContent[i] = pointer[i];
}

MessageHeader &Message::header()
{
    return mHeader;
}

const MessageHeader &Message::header() const
{
    return mHeader;
}

Message::ContentPointer &Message::content()
{
    return mContent;
}

const Message::ContentPointer &Message::content() const
{
    return mContent;
}

MutableBuffer Message::getContentBuffer()
{
    return boost::asio::buffer(mContent.get(), mHeader.length);
}

ConstBuffer Message::getContentBuffer() const
{
    return boost::asio::buffer(mContent.get(), mHeader.length);
}

// ^^^ //  General implementation  // ^^^ //
// vvv // Interface implementation // vvv //

uint8_t Message::getPurpose() const
{
    return mHeader.purposeByte;
}

uint8_t Message::getTaskId() const
{
    return mHeader.taskId;
}
uint32_t Message::getContentLength() const
{
    return mHeader.length;
}

const uint8_t* Message::getContentRawData() const
{
    return mContent.get();
}

ConstBufferArray<3> Message::getHeaderBufferSequence() const
{
    return mHeader.getBufferSequence();
}

ConstBufferVector Message::getBufferSequence() const
{
    auto headerSeq = mHeader.getBufferSequence();
    ConstBufferVector vec(headerSeq.begin(), headerSeq.end());
    if (mHeader.length != 0)
        vec.push_back(getContentBuffer());

    return vec;
}