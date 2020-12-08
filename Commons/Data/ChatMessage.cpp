#include "ChatMessage.hpp"

using namespace Commons::Data;

ISendable::ConstBufferVector ChatMessage::getConstDataSequence() const
{
    ConstBufferVector vec;
    vec.push_back(Buffer::primitiveType(mSenderId));
    vec.push_back(Buffer::primitiveType(mChatId));
    vec.push_back(Buffer::primitiveType(mTimeCreated));
    vec.push_back(Buffer::primitiveType(mTimeUpdated));
    vec.push_back(Buffer::primitiveType(mStatus));

    vec.push_back(Buffer::stdString(mText));

    return vec;
}

void ChatMessage::fillFromBuffer(const ConstBuffer& buffer)
{
    BufferDecomposer decomposer(buffer);
    mSenderId = decomposer.get<int4>();
    mChatId = decomposer.get<int4>();
    mTimeCreated = decomposer.get<time_t>();
    mTimeCreated = decomposer.get<time_t>();
    mStatus = decomposer.get<int1>();
    mText = decomposer.get<std::string>();
}

size_t ChatMessage::bytes() const
{
    return
        sizeof (mSenderId) +
        sizeof (mChatId) +
        sizeof (mTimeCreated) +
        sizeof (mTimeUpdated) +
        sizeof (mStatus) +
        mText.size() + 1 +
        0
        ;
}

int4 ChatMessage::getSenderId() const
{
    return mSenderId;
}

void ChatMessage::setSenderId(int4 sender)
{
    mSenderId = sender;
}

int4 ChatMessage::getChatId() const
{
    return mChatId;
}

void ChatMessage::setChatId(int4 chatId)
{
    mChatId = chatId;
}

const std::string& ChatMessage::getText() const
{
    return mText;
}

void ChatMessage::setText(const std::string& text)
{
    mText = text;
}

time_t ChatMessage::getTimeCreated() const
{
    return mTimeCreated;
}

void ChatMessage::setTimeCreated(time_t timeCreated)
{
    mTimeCreated = timeCreated;
}

time_t ChatMessage::getTimeUpdated() const
{
    return mTimeUpdated;
}

void ChatMessage::setTimeUpdated(time_t timeUpdated)
{
    mTimeUpdated = timeUpdated;
}

int1 ChatMessage::getStatus() const
{
    return mStatus;
}

void ChatMessage::setStatus(int1 status)
{
    mStatus = status;
}
