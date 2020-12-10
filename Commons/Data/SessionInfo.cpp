#include "SessionInfo.hpp"

using namespace Commons::Data;

size_t SessionInfo::fillFromBuffer(const ConstBuffer& buffer)
{
    BufferDecomposer decomposer(buffer);
    size_t bytes_before = decomposer.bytesLeft();

    decomposer
        .extract(mId)
        .extract(mIp4)
        .extract(mLastActivity)
        .extract(mUserId);

    size_t bytes_after = decomposer.bytesLeft();

    return bytes_before - bytes_after;
}

ISendable::ConstBufferVector SessionInfo::getConstDataSequence() const
{
    ConstBufferVector seq;
    BufferComposer composer(seq);

    composer
        .append(mId)
        .append(mIp4)
        .append(mLastActivity)
        .append(mUserId);

    return Commons::Data::ISendable::ConstBufferVector();
}

size_t SessionInfo::bytes() const
{
    return
        sizeof (mId) +
        sizeof (mIp4) +
        sizeof (mLastActivity) +
        sizeof (mUserId) +
        0
        ;
}

int4 SessionInfo::getId() const
{
    return mId;
}

void SessionInfo::setId(int4 id)
{
    mId = id;
}

int4 SessionInfo::getIp4() const
{
    return mIp4;
}

void SessionInfo::setIp4(int4 ip4)
{
    mIp4 = ip4;
}

time_t SessionInfo::getLastActivity() const
{
    return mLastActivity;
}

void SessionInfo::setLastActivity(time_t lastActivity)
{
    mLastActivity = lastActivity;
}

int4 SessionInfo::getUserId() const
{
    return mUserId;
}

void SessionInfo::setUserId(int4 userId)
{
    mUserId = userId;
}

const std::string& SessionInfo::getHash() const
{
    return mHash;
}

void SessionInfo::setHash(const std::string& hash)
{
    mHash = hash;
}
