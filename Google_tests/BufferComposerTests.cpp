#include "gtest/gtest.h"
#include <Data/BufferComposer.hpp>
using namespace  Commons::Data;

TEST(BufferComTest, vectorSize) {
    const char* c_string = "a";
    std::string cpp_string = "b";
    int test = 1;
    std::vector<ConstBuffer> vec;
    BufferComposer composer(vec);
    composer.add(c_string);
    composer.add(cpp_string);
    composer.add(test);
    EXPECT_EQ(vec.size(),3);
}

TEST(BufferComTest, vectorOrder) {
    const char* c_string = "a";
    std::string cpp_string = "b";
    int test = 1;
    std::vector<ConstBuffer> vec;
    BufferComposer composer(vec);
    composer.add(c_string);
    composer.add(cpp_string);
    composer.add(test);

    EXPECT_STREQ(static_cast<const char*>(vec[0].data()),c_string);
    EXPECT_STREQ(static_cast<const char*>(vec[1].data()),cpp_string.c_str());
    EXPECT_EQ(*static_cast<const int*>(vec[2].data()),test);
}

TEST(BufferComTest, vectorAppend) {
    int test1 = 1;
    int test2 = 2;
    int test3 = 3;
    std::vector<ConstBuffer> vec;
    BufferComposer composer(vec);
    composer.append(test1).append(test2).append(test3);

    EXPECT_EQ(vec.size(),3);

    EXPECT_EQ(*static_cast<const int*>(vec[0].data()),test1);
    EXPECT_EQ(*static_cast<const int*>(vec[1].data()),test2);
    EXPECT_EQ(*static_cast<const int*>(vec[2].data()),test3);
}

