//TestTag.cpp
//Copyright 2G Robotics Inc., 2021

#include <gtest/gtest.h>
#include "EXIFTags/Tag.h"
#include "EXIFTags/TagConstants.h"
#include <cstdint>
#include <string>
#include <memory>

namespace tg {
namespace tags {

TEST ( TagTest, UINT32) {
    std::unique_ptr <Tag> tag = Tag::tagFactory(Constants::IMAGE_WIDTH);

    GTEST_ASSERT_NE(tag, nullptr);

    uint32_t test_in (2048);
    dynamic_cast <Tag_UINT32*>(tag.get())->setData(test_in);

    ASSERT_EQ (dynamic_cast <Tag_UINT32*>(tag.get())->getData(), test_in);
} 

TEST ( TagTest, UINT16) {
    std::unique_ptr <Tag> tag = Tag::tagFactory(Constants::PHOTOMETRIC_INTERPOLATION);

    GTEST_ASSERT_NE(tag, nullptr);

    uint16_t test_in (1);
    dynamic_cast <Tag_UINT16*>(tag.get())->setData(test_in);

    ASSERT_EQ (dynamic_cast <Tag_UINT16*>(tag.get())->getData(), test_in);
} 

TEST ( TagTest, UINT8) {
    std::unique_ptr <Tag> tag = Tag::tagFactory(Constants::GPS_ALTITUDE_REF);

    GTEST_ASSERT_NE(tag, nullptr);

    uint8_t test_in (1);
    dynamic_cast <Tag_UINT8*>(tag.get())->setData(test_in);

    ASSERT_EQ (dynamic_cast <Tag_UINT8*>(tag.get())->getData(), test_in);
} 

TEST ( TagTest, DOUBLE) {
    std::unique_ptr <Tag> tag = Tag::tagFactory(Constants::EXPOSURE_TIME);

    GTEST_ASSERT_NE(tag, nullptr);

    double test_in (18.23456);
    dynamic_cast <Tag_DOUBLE*>(tag.get())->setData(test_in);

    ASSERT_DOUBLE_EQ (dynamic_cast <Tag_DOUBLE*>(tag.get())->getData(), test_in);
} 

TEST ( TagTest, UINT16_ARRAY) {
    std::unique_ptr <Tag> tag = Tag::tagFactory(Constants::PIXEL_SIZE);

    GTEST_ASSERT_NE(tag, nullptr);

    std::vector<uint16_t> test_in {36, 37};
    dynamic_cast <Tag_UINT16_ARRAY*>(tag.get())->setData(test_in);

    ASSERT_EQ (dynamic_cast <Tag_UINT16_ARRAY*>(tag.get())->getData()[0], test_in[0]);
    ASSERT_EQ (dynamic_cast <Tag_UINT16_ARRAY*>(tag.get())->getData()[1], test_in[1]);
} 

TEST ( TagTest, DOUBLE_ARRAY) {
    std::unique_ptr <Tag> tag = Tag::tagFactory(Constants::CAMERA_MATRIX);

    GTEST_ASSERT_NE(tag, nullptr);

    std::vector<double> test_in {1.00, 2.00, 3.00, 4.00};
    dynamic_cast <Tag_DOUBLE_ARRAY*>(tag.get())->setData(test_in);

    ASSERT_EQ (dynamic_cast <Tag_DOUBLE_ARRAY*>(tag.get())->getData()[0], test_in[0]);
    ASSERT_EQ (dynamic_cast <Tag_DOUBLE_ARRAY*>(tag.get())->getData()[1], test_in[1]);
    ASSERT_EQ (dynamic_cast <Tag_DOUBLE_ARRAY*>(tag.get())->getData()[2], test_in[2]);
    ASSERT_EQ (dynamic_cast <Tag_DOUBLE_ARRAY*>(tag.get())->getData()[3], test_in[3]);
} 

TEST ( TagTest, STRING) {
    std::unique_ptr <Tag> tag = Tag::tagFactory(Constants::SOFTWARE);

    GTEST_ASSERT_NE(tag, nullptr);

    std::string test_in ("ViewLS");
    dynamic_cast <Tag_STRING*>(tag.get())->setData(test_in);

    ASSERT_EQ (dynamic_cast <Tag_STRING*>(tag.get())->getData(), test_in);
} 


}
}