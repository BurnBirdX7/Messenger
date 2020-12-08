#include "ChatInfo.hpp"

using namespace Commons::Data;

ChatInfo::ConstBufferVector ChatInfo::getConstDataSequence() const
{
    ConstBufferVector vec;

    vec.push_back(Buffer::primitiveType(mId));
    vec.push_back(Buffer::stdString(mTitle));
    vec.push_back(Buffer::primitiveType(mIsDirect));
    vec.push_back(Buffer::primitiveType(mTimeUpdated));
    vec.push_back(Buffer::stdString(mNickname));

    return vec;
}

size_t ChatInfo::bytes() const
{
    return
            sizeof (mId) +
            sizeof (mIsDirect) +
            sizeof (mTimeUpdated) +
            mTitle.length() + 1 +
            mNickname.length() + 1 +
            0
            ;
}


void ChatInfo::fillFromBuffer(const ConstBuffer& buffer)
{
    BufferDecomposer decomposer(buffer);

    mId = decomposer.get<int4>();
    mTitle = decomposer.get<std::string>();
    mIsDirect = decomposer.get<bool>();
    mTimeUpdated = decomposer.get<time_t>();
    mNickname = decomposer.get<std::string>();
}

int4 ChatInfo::getId() const
{
    return mId;
}

void ChatInfo::setId(int4 id)
{
    mId = id;
}

const std::string& ChatInfo::getTitle() const
{
    return mTitle;
}

void ChatInfo::setTitle(const std::string& title)
{
    mTitle = title;
}

bool ChatInfo::isDirect() const
{
    return mIsDirect;
}

void ChatInfo::setIsDirect(bool isDirect)
{
    mIsDirect = isDirect;
}

time_t ChatInfo::getTimeUpdated() const
{
    return mTimeUpdated;
}

void ChatInfo::setTimeUpdated(time_t timeUpdated)
{
    mTimeUpdated = timeUpdated;
}

const std::string& ChatInfo::getNickname() const
{
    return mNickname;
}

void ChatInfo::setNickname(const std::string& nickname)
{
    mNickname = nickname;
}

