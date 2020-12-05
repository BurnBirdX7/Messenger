#include "ChatMessage.hpp"

using namespace Commons::Data;

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
