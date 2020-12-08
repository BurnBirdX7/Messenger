#include "ChatMessage.hpp"

using namespace Commons::Data;

ISendable::ConstBufferVector ChatMessage::getConstDataSequence() const
{
    ConstBufferVector vec;
    BufferComposer composer(vec);

    /*
    vec.push_back(Buffer::primitiveType(mId));
    vec.push_back(Buffer::primitiveType(mSenderId));
    vec.push_back(Buffer::primitiveType(mChatId));
    vec.push_back(Buffer::primitiveType(mTimeCreated));
    vec.push_back(Buffer::primitiveType(mTimeUpdated));
    vec.push_back(Buffer::primitiveType(mStatus));

    vec.push_back(Buffer::stdString(mText));
    */

    composer
        .append(mId)
        .append(mSenderId)
        .append(mChatId)
        .append(mTimeCreated)
        .append(mTimeUpdated)
        .append(mStatus)
        .append(mText);

    return vec;
}

void ChatMessage::fillFromBuffer(const ConstBuffer& buffer)
{
    BufferDecomposer decomposer(buffer);
    /*
    mId = decomposer.get<int4>();
    mSenderId = decomposer.get<int4>();
    mChatId = decomposer.get<int4>();
    mTimeCreated = decomposer.get<time_t>();
    mTimeUpdated = decomposer.get<time_t>();
    mStatus = decomposer.get<int1>();
    mText = decomposer.get<std::string>();
    */

    decomposer
        .extract(mId)
        .extract(mSenderId)
        .extract(mChatId)
        .extract(mTimeCreated)
        .extract(mTimeUpdated)
        .extract(mStatus)
        .extract(mText);

}

size_t ChatMessage::bytes() const
{
    return
        sizeof (mId) +
        sizeof (mSenderId) +
        sizeof (mChatId) +
        sizeof (mTimeCreated) +
        sizeof (mTimeUpdated) +
        sizeof (mStatus) +
        mText.size() + 1 +
        0
        ;
}

int4 ChatMessage::getId() const
{
    return mId;
}

void ChatMessage::setId(int4 id)
{
    mId = id;
}

int4 ChatMessage::getSenderId() const
{
    return mSenderId;
}

void ChatMessage::setSenderId(int4 senderId)
{
    mSenderId = senderId;
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
