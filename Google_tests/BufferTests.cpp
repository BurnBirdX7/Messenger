#include "gtest/gtest.h"
#include <Data/Buffer.hpp>
#include <DataBuffers.hpp>

TEST(BufferTest, StringSize) {
    std::string str = "test";
    Commons::Data::ConstBuffer buffer = Commons::Data::Buffer::string(str);
    auto strData = static_cast<const char*>(buffer.data());
    EXPECT_EQ(buffer.size(),str.length()+1);
}

TEST(BufferTest, StringData) {
    std::string str = "test";
    Commons::Data::ConstBuffer buffer = Commons::Data::Buffer::string(str);
    auto strData = static_cast<const char*>(buffer.data());
    EXPECT_STREQ(strData, str.c_str());
}

TEST(BufferTest, CharSize) {
    const char* str = "test";
    Commons::Data::ConstBuffer buffer = Commons::Data::Buffer::string(str);
    auto strData = static_cast<const char*>(buffer.data());
    EXPECT_EQ(buffer.size(),strlen(str)+1);
}

TEST(BufferTest, CharData) {
    const char* str = "test";
    Commons::Data::ConstBuffer buffer = Commons::Data::Buffer::string(str);
    auto strData = static_cast<const char*>(buffer.data());
    EXPECT_STREQ(strData, str);
}






