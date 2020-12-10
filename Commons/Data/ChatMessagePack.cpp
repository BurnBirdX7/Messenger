#include "ChatMessagePack.hpp"

using namespace Commons::Data;

ChatMessagePack::ChatMessagePack(size_t count)
{
    mPack.reserve(count);
}

ISendable::ConstBufferVector ChatMessagePack::getConstDataSequence() const
{
    ConstBufferVector seq;
    size_t toReserve = mPack.size() * ChatMessage::BUFFERS_COUNT;
    seq.reserve(toReserve);
    BufferComposer composer(seq);

    for (const auto& msg : mPack)
        composer.add(msg.getConstDataSequence());

    assert(toReserve == seq.size());

    return seq;
}

size_t ChatMessagePack::bytes() const
{
    size_t bytes = 0;

    for (const auto& msg : mPack)
        bytes += msg.bytes();

    return bytes;
}

size_t ChatMessagePack::fillFromBuffer(const ConstBuffer& ref_buffer)
{
    ConstBuffer buffer(ref_buffer);
    size_t before_read = buffer.size();

    try {
        while (buffer.size() > 0) {
            mPack.emplace_back();                           // a new message
            auto& last = --mPack.end();                     // filling it
            size_t bytes = last->fillFromBuffer(buffer);    //

            buffer += bytes;
        }
    }
    catch (const std::runtime_error& error) {
        auto& last = --mPack.end();
        mPack.erase(last);
    }

    size_t after_read = buffer.size();

    return before_read - after_read;
}

void ChatMessagePack::addMessage(const ChatMessage& message)
{
    mPack.push_back(message);
}

const ChatMessage& ChatMessagePack::at(size_t index) const
{
    return mPack.at(index);
}

size_t ChatMessagePack::count() const
{
    return mPack.size();
}

const std::vector<ChatMessage>& ChatMessagePack::getVector() const
{
    return mPack;
}
