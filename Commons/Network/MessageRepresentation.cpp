//
// Created by artem on 14.11.2020.
//

#include "MessageRepresentation.hpp"

using namespace Commons::Network;

MessageRepresentation::MessageRepresentation(const uint8_t &purpose,
                                             const uint8_t &taskId,
                                             ConstBuffer content)
    : mLength(content.size())
    , mPurpose(boost::asio::buffer(&purpose, sizeof(purpose)))
    , mTaskId(boost::asio::buffer(&taskId, sizeof(taskId)))
    , mContent(content)
{}

MessageRepresentation::MessageRepresentation(const uint8_t &purpose,
                                             const uint8_t &taskId)
    : mLength(0)
    , mContent(nullptr, 0)
    , mPurpose(boost::asio::buffer(&purpose, sizeof(purpose)))
    , mTaskId(boost::asio::buffer(&taskId, sizeof(taskId)))
{}

ConstBufferVector MessageRepresentation::getBufferSequence() const {

    std::vector<ConstBuffer> seq;

    seq.push_back(boost::asio::buffer(&mLength, sizeof(mLength)));

    seq.push_back(mPurpose);
    seq.push_back(mTaskId);

    if (mLength != 0)
        seq.push_back(mContent);

    return seq;
}

// ^^^ //  General implementation  // ^^^ //
// vvv // Interface implementation // vvv //

uint8_t MessageRepresentation::getPurpose() const
{
    return *static_cast<const uint8_t*>(mPurpose.data());
}

uint8_t MessageRepresentation::getTaskId() const
{
    return *static_cast<const uint8_t*>(mTaskId.data());
}

uint32_t MessageRepresentation::getContentLength() const
{
    return mLength;
}

const uint8_t* MessageRepresentation::getContentRawData() const
{
    return static_cast<const uint8_t*>(mContent.data());
}

ConstBufferArray<3> MessageRepresentation::getHeaderBufferSequence() const
{
    return {boost::asio::buffer(&mLength, sizeof(mLength)), mPurpose, mTaskId};
}

ConstBuffer MessageRepresentation::getContentBuffer() const
{
    return mContent;
}
