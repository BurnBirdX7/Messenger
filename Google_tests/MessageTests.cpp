#include "gtest/gtest.h"
#include <Network.hpp>
#include <Data.hpp>

using namespace Commons::Network;

inline Message newMessage(){
    const char* str = "HELLO WORLD";

    MessageHeader header{};
    header.length = strlen(str);
    header.purposeByte = Purpose::HELLO;
    header.taskId = 1;

    return Message (header, reinterpret_cast<const uint8_t *>(str), strlen(str));
}

TEST(MessageTestHeader, testLenght) {
    Message message = newMessage();
    EXPECT_EQ(message.header().length, message.getContentLength());
}

TEST(MessageTestHeader, testPurpose) {
    Message message = newMessage();
    EXPECT_EQ(message.header().purposeByte, message.getPurpose());
}

TEST(MessageTestHeader, testTaskId) {
    Message message = newMessage();
    EXPECT_EQ(message.header().taskId, message.getTaskId());
}

TEST(MessageTestData, testBufferContentSize) {
    Message message = newMessage();
    Commons::Data::ConstBuffer buffer = message.getContentBuffer();
    EXPECT_EQ(message.getContentBuffer().size(), message.header().length);
}

TEST(MessageTestData, testBufferContentData) {
    Message message = newMessage();
    Commons::Data::ConstBuffer buffer = message.getContentBuffer();
    auto strData = reinterpret_cast<const uint8_t *>(buffer.data());
    EXPECT_EQ(strData, message.getContentRawData());
}
