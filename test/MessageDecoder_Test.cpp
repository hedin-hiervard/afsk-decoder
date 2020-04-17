#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "MessageDecoder.h"

using namespace std;

class MessageDecoderTest : public testing::Test {
public:

    MessageDecoder objectUnderTest;

    void printErrors(const MessageDecoder::Errors& errors) {
        for(const auto& e: errors) {
            cout << "error: " << e.message << " at byte: " << e.position << endl;
        }
    }
};

TEST_F(MessageDecoderTest, shouldDecodeSimpleMessage)
{
    MessageDecoder::Messages messages;
    MessageDecoder::Errors errors;

    objectUnderTest.decode({
        0xff, 0xff, 0xff,
        0x42, 0x03,
        0xaa, 0xaa, 0xaa,
        0xaa, 0xaa, 0xaa,
        0xaa, 0xaa, 0xaa,
        0xaa, 0xaa, 0xaa,
        0xaa, 0xaa, 0xaa,
        0xaa, 0xaa, 0xaa,
        0xaa, 0xaa, 0xaa,
        0xaa, 0xaa, 0xaa,
        0xaa, 0xaa, 0xaa,
        0xaa, 0xaa, 0xaa,
        0xbb
    },
        back_inserter(messages),
        back_inserter(errors)
    );

    EXPECT_EQ(1, messages.size());
    EXPECT_EQ(0, errors.size());
}


TEST_F(MessageDecoderTest, shouldDecodeRealWorldMessage)
{
    MessageDecoder::Messages messages;
    MessageDecoder::Errors errors;

    objectUnderTest.decode({
        0xff, 0xff, 0xff, 0xff,
        0x42, 0x3,
        0x49, 0x74, 0x20, 0x77, 0x61, 0x73, 0x20, 0x74, 0x68, 0x65,
        0x20, 0x57, 0x68, 0x69, 0x74, 0x65, 0x20, 0x52, 0x61, 0x62,
        0x62, 0x69, 0x74, 0x2c, 0x20, 0x74, 0x72, 0x6f, 0x74, 0x74,
        0xa7,
    },
        back_inserter(messages),
        back_inserter(errors)
    );

    EXPECT_EQ(1, messages.size());
    EXPECT_EQ(0, errors.size());
    printErrors(errors);

}