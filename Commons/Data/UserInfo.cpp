#include "UserInfo.hpp"

using namespace Commons::Data;

UserInfo::ConstBufferVector UserInfo::getConstDataSequence() const
{
    ConstBufferVector seq;
    BufferComposer composer(seq);
    composer
        .append(mId)
        .append(mName)
        .append(mNickname);

    return seq;
}

size_t UserInfo::bytes() const
{
    return
        sizeof (mId) +
        mName.size() + 1 +
        mNickname.size() + 1 +
        0
        ;
}

size_t UserInfo::fillFromBuffer(const ConstBuffer& buffer)
{
    BufferDecomposer decomposer(buffer);
    size_t before_read = decomposer.bytesLeft();

    decomposer
        .extract(mId)
        .extract(mName)
        .extract(mNickname);

    size_t after_read = decomposer.bytesLeft();

    return before_read - after_read;
}

int4 UserInfo::getId() const
{
    return mId;
}

void UserInfo::setId(int4 id)
{
    mId = id;
}

const std::string& UserInfo::getName() const
{
    return mName;
}

void UserInfo::setName(const std::string& name)
{
    UserInfo::mName = name;
}

const std::string& UserInfo::getNickname() const
{
    return mNickname;
}

void UserInfo::setNickname(const std::string& nickname)
{
    UserInfo::mNickname = nickname;
}

const std::string& UserInfo::getPasswordHash() const
{
    return mPasswordHash;
}

void UserInfo::setPasswordHash(const std::string& passwordHash)
{
    mPasswordHash = passwordHash;
}
