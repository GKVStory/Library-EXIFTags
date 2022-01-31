// TestTagConstants.cpp
// Copyright Voyis Inc., 2021

#include "EXIFTags/TagConstants.h"
#include <cstdint>
#include <gtest/gtest.h>
#include <string>

namespace tg {
namespace tags {

TEST(TagConstantsTest, CheckLatLonConversions) {
    const double test_decdeg = 83.1234567;
    const double test_deg = 83.0;
    const double test_minute = 7.0;
    const double test_second = 24.444120000019886;
    double deg(0.0), minute(0.0), second(0.0), decdeg(0);

    decdeg = Constants::DMSToDeg(test_deg, test_minute, test_second);

    ASSERT_DOUBLE_EQ(decdeg, test_decdeg);

    Constants::degToDMS(deg, minute, second, test_decdeg);
    ASSERT_DOUBLE_EQ(test_deg, deg);
    ASSERT_DOUBLE_EQ(test_minute, minute);
    ASSERT_DOUBLE_EQ(test_second, second);
}

} // namespace tags
} // namespace tg