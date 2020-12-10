#include "ChatInfo.hpp"

using namespace Commons::Data;

ChatInfo::ConstBufferVector ChatInfo::getConstDataSequence() const
{
    ConstBufferVector vec;
    BufferComposer composer(vec);

    /*
    vec.push_back(Buffer::primitiveType(mId));
    vec.push_back(Buffer::stdString(mTitle));
    vec.push_back(Buffer::primitiveType(mIsDirect));
    vec.push_back(Buffer::primitiveType(mTimeUpdated));
    vec.push_back(Buffer::stdString(mNickname));
    */

    composer
        .append(mId)
        .append(mTitle)
        .append(mIsDirect)
        .append(mTimeUpdated)
        .append(mNickname);

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


size_t ChatInfo::fillFromBuffer(const ConstBuffer& buffer)
{
    BufferDecomposer decomposer(buffer);

    size_t before_read = decomposer.bytesLeft();

    decomposer
        .extract(mId)
        .extract(mTitle)
        .extract(mIsDirect)
        .extract(mTimeUpdated)
        .extract(mNickname);

    size_t after_read = decomposer.bytesLeft();

    return before_read - after_read;
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

