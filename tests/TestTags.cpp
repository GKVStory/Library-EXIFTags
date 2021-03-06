// TestTags.cpp
// Copyright Voyis Inc., 2021

#include "EXIFTags/Tags.h"
#include "TestConstants.h"
#include <cstdint>
#include <gtest/gtest.h>
#include <string>

namespace tg {
namespace tags {

TEST(TagsTest, GenerateEmptyTagsClass_CheckDefaultBehaviour) {
    Tags tags;

    ASSERT_EQ(tags.subfileType(), Tags::FULL_RESOLUTION_IMAGE);

    ASSERT_EQ(tags.imageHeight(), 0);
    tags.imageHeight(1024);
    ASSERT_EQ(tags.imageHeight(), 1024);

    ASSERT_EQ(tags.imageWidth(), 0);
    tags.imageWidth(2048);
    ASSERT_EQ(tags.imageWidth(), 2048);

    ASSERT_EQ(tags.bitsPerSample()[0], 8);
    tags.bitsPerSample(std::vector<uint16_t>{8, 8, 8});
    ASSERT_EQ(tags.bitsPerSample().size(), 3);
    ASSERT_EQ(tags.bitsPerSample()[0], 8);

    ASSERT_EQ(tags.compression(), Tags::COMPRESSION_EXIF_NONE);
    tags.compression(Tags::COMPRESSION_EXIF_JPEG);
    ASSERT_EQ(tags.compression(), Tags::COMPRESSION_EXIF_JPEG);

    ASSERT_EQ(tags.photometricInterpolation(), Tags::PHOTOMETRIC_EXIF_MINISBLACK);
    tags.photometricInterpolation(Tags::PHOTOMETRIC_EXIF_RGB);
    ASSERT_EQ(tags.photometricInterpolation(), Tags::PHOTOMETRIC_EXIF_RGB);

    ASSERT_EQ(tags.imageDescription(), "");
    tags.imageDescription("Test description!");
    ASSERT_EQ(tags.imageDescription(), "Test description!");

    ASSERT_EQ(tags.make(), Constants::DEFAULT_MAKE);

    ASSERT_EQ(tags.model(), "");
    tags.model("Test model!");
    ASSERT_EQ(tags.model(), "Test model!");

    ASSERT_EQ(tags.stripOffsets().size(), 0);
    tags.stripOffsets(std::vector<uint32_t>{440});
    ASSERT_EQ(tags.stripOffsets()[0], 440);

    ASSERT_EQ(tags.orientation(), Tags::ORIENTATION_EXIF_TOPLEFT);

    ASSERT_EQ(tags.samplesPerPixel(), 1);
    tags.samplesPerPixel(3);
    ASSERT_EQ(tags.samplesPerPixel(), 3);

    ASSERT_EQ(tags.rowsPerStrip(), 0);
    tags.rowsPerStrip(56);
    ASSERT_EQ(tags.rowsPerStrip(), 56);

    ASSERT_EQ(tags.stripByteCount().size(), 0);
    tags.stripByteCount(std::vector<uint32_t>{1025});
    ASSERT_EQ(tags.stripByteCount()[0], 1025);

    ASSERT_EQ(tags.planarConfiguration(), Tags::PLANARCONFIG_EXIF_CONTIG);

    ASSERT_EQ(tags.software(), "");
    tags.software("Test software!");
    ASSERT_EQ(tags.software(), "Test software!");

    ASSERT_DOUBLE_EQ(tags.exposureTime(), 0.0);
    tags.exposureTime(3.0);
    ASSERT_DOUBLE_EQ(tags.exposureTime(), 3.0);

    ASSERT_DOUBLE_EQ(tags.fNumber(), 0.0);
    tags.fNumber(5.0);
    ASSERT_DOUBLE_EQ(tags.fNumber(), 5.0);

    ASSERT_EQ(tags.dateTime(), 0);
    tags.dateTime(1614632629005001);
    ASSERT_EQ(tags.dateTime(), 1614632629005001);

    ASSERT_DOUBLE_EQ(tags.subjectDistance(), 0.0);
    tags.subjectDistance(6.0);
    ASSERT_DOUBLE_EQ(tags.subjectDistance(), 6.0);

    ASSERT_EQ(tags.lightSource(), Tags::LIGHTSOURCE_UNKNOWN);
    tags.lightSource(Tags::LIGHTSOURCE_BLUELED);
    ASSERT_EQ(tags.lightSource(), Tags::LIGHTSOURCE_BLUELED);

    ASSERT_EQ(tags.flash(), Tags::FLASH_NO_FLASH);
    tags.flash(Tags::FLASH_FIRED);
    ASSERT_EQ(tags.flash(), Tags::FLASH_FIRED);

    ASSERT_DOUBLE_EQ(tags.focalLength(), 0.0);
    tags.focalLength(1.2);
    ASSERT_DOUBLE_EQ(tags.focalLength(), 1.2);

    ASSERT_EQ(tags.colourSpace(), Tags::COLOURSPACE_sRGB);
    tags.colourSpace(Tags::COLOURSPACE_ADOBE_RGB);
    ASSERT_EQ(tags.colourSpace(), Tags::COLOURSPACE_ADOBE_RGB);

    ASSERT_DOUBLE_EQ(tags.flashEnergy(), 0.0);
    tags.flashEnergy(67.0);
    ASSERT_DOUBLE_EQ(tags.flashEnergy(), 67.0);

    ASSERT_EQ(tags.serialNumber(), "");
    tags.serialNumber("Test serial number");
    ASSERT_EQ(tags.serialNumber(), "Test serial number");

    ASSERT_EQ(tags.lensModel(), "");
    tags.lensModel("Test lens model");
    ASSERT_EQ(tags.lensModel(), "Test lens model");

    ASSERT_DOUBLE_EQ(tags.indexOfRefraction(), Constants::DEFAULT_INDEX);
    tags.indexOfRefraction(1.345);
    ASSERT_DOUBLE_EQ(tags.indexOfRefraction(), 1.345);

    ASSERT_DOUBLE_EQ(tags.viewportIndex(), Constants::DEFAULT_VIEWPORT_INDEX);
    tags.viewportIndex(1.4);
    ASSERT_DOUBLE_EQ(tags.viewportIndex(), 1.4);

    ASSERT_DOUBLE_EQ(tags.viewportThickness(), 0);
    tags.viewportThickness(0.25);
    ASSERT_DOUBLE_EQ(tags.viewportThickness(), 0.25);

    ASSERT_DOUBLE_EQ(tags.viewportDistance(), 0);
    tags.viewportDistance(0.26);
    ASSERT_DOUBLE_EQ(tags.viewportDistance(), 0.26);

    ASSERT_FALSE(tags.vignetting());
    tags.vignetting(true);
    ASSERT_TRUE(tags.vignetting());

    ASSERT_EQ(tags.viewportType(), Tags::VIEWPORT_FLAT);
    tags.viewportType(Tags::VIEWPORT_DOMED);
    ASSERT_EQ(tags.viewportType(), Tags::VIEWPORT_DOMED);

    ASSERT_EQ(tags.enhancement(), Tags::ENHANCEMENT_NONE);
    tags.enhancement(Tags::ENHANCEMENT_BASIC_COLOUR);
    ASSERT_EQ(tags.enhancement(), Tags::ENHANCEMENT_BASIC_COLOUR);

    ASSERT_TRUE(tags.pixelSize().empty());
    const std::vector<uint16_t> pixel_vec = {3600, 3700};
    tags.pixelSize(pixel_vec);
    ASSERT_EQ(tags.pixelSize(), pixel_vec);

    ASSERT_TRUE(tags.matrixNavToCamera().size() == 16);
    ASSERT_EQ(tags.matrixNavToCamera(), Constants::DEFAULT_TRANSFORM);
    const std::vector<double> nav_vec = {
        1.0, 2.0, 3.0, 4.0, 5.0, 1.0, 6.0, 7.0, 8.0, 9.0, 1.0, 11.0, 12.0, 13.0, 14.0, 1.0};
    tags.matrixNavToCamera(nav_vec);
    ASSERT_EQ(tags.matrixNavToCamera(), nav_vec);

    ASSERT_EQ(tags.imageNumber(), 0);
    tags.imageNumber(3);
    ASSERT_EQ(tags.imageNumber(), 3);

    ASSERT_DOUBLE_EQ(tags.waterDepth(), 0);
    tags.waterDepth(10.5);
    ASSERT_DOUBLE_EQ(tags.waterDepth(), 10.5);

    ASSERT_EQ(tags.bayerPattern(), Tags::BAYER_GRAYSCALE);
    tags.bayerPattern(Tags::BAYER_BG2BGR);
    ASSERT_EQ(tags.bayerPattern(), Tags::BAYER_BG2BGR);

    ASSERT_DOUBLE_EQ(tags.frameRate(), 0.0);
    tags.frameRate(45.0);
    ASSERT_DOUBLE_EQ(tags.frameRate(), 45.0);

    ASSERT_TRUE(tags.cameraMatrix().size() == 4);
    ASSERT_EQ(tags.cameraMatrix(), Constants::DEFAULT_CAM_MATRIX);
    const std::vector<double> cam_vec = {2000.0, 2001.0, 1036, 738.2};
    tags.cameraMatrix(cam_vec);
    ASSERT_EQ(tags.cameraMatrix(), cam_vec);

    ASSERT_TRUE(tags.distortion().size() == 5);
    ASSERT_EQ(tags.distortion(), Constants::DEFAULT_DISTORTION);
    const std::vector<double> dist_vec = {-0.1, 0.2, -0.3, 0.5, -0.5};
    tags.distortion(dist_vec);
    ASSERT_EQ(tags.distortion(), dist_vec);

    ASSERT_TRUE(tags.pose().size() == 3);
    ASSERT_EQ(tags.pose(), Constants::DEFAULT_POSE);
    const std::vector<double> pose_vec = {1.0, 2.3, 3.4};
    tags.pose(pose_vec);
    ASSERT_EQ(tags.pose(), pose_vec);

    ASSERT_DOUBLE_EQ(tags.vehicleAltitude(), 0.0);
    tags.vehicleAltitude(10.87);
    ASSERT_DOUBLE_EQ(tags.vehicleAltitude(), 10.87);

    ASSERT_EQ(tags.latitudeRef(), Tags::LATITUDEREF_NORTH);
    tags.latitudeRef(Tags::LATITUDEREF_SOUTH);
    ASSERT_EQ(tags.latitudeRef(), Tags::LATITUDEREF_SOUTH);

    ASSERT_DOUBLE_EQ(tags.latitude(), 0.0);
    tags.latitude(83.1245678);
    ASSERT_DOUBLE_EQ(tags.latitude(), 83.1245678);

    ASSERT_EQ(tags.longitudeRef(), Tags::LONGITUDEREF_EAST);
    tags.longitudeRef(Tags::LONGITUDEREF_WEST);
    ASSERT_EQ(tags.longitudeRef(), Tags::LONGITUDEREF_WEST);

    ASSERT_DOUBLE_EQ(tags.longitude(), 0.0);
    tags.longitude(43.67890);
    ASSERT_DOUBLE_EQ(tags.longitude(), 43.67890);

    ASSERT_EQ(tags.altitudeRef(), Tags::ALTITUDEREF_ABOVE_SEA_LEVEL);
    tags.altitudeRef(Tags::ALTITUDEREF_BELOW_SEA_LEVEL);
    ASSERT_EQ(tags.altitudeRef(), Tags::ALTITUDEREF_BELOW_SEA_LEVEL);

    ASSERT_DOUBLE_EQ(tags.altitude(), 0.0);
    tags.altitude(10.765);
    ASSERT_DOUBLE_EQ(tags.altitude(), 10.765);

    ASSERT_EQ(tags.ppsTime(), 0);
    tags.ppsTime(1614632629005011);
    ASSERT_EQ(tags.ppsTime(), 1614632629005011);

    tags.ppsTime(1714632629005021);
    tags.dateTime(1714632629005021);
    ASSERT_EQ(tags.ppsTime(), 1714632629005021);
    ASSERT_EQ(tags.ppsTime(), tags.dateTime());
}

TEST(TagsTest, ParseJpegFile) {
    Tags tags;
    std::string error_message;

    ASSERT_FALSE(tags.loadHeader("DoesntExist.jpg", error_message));
    // ASSERT_EQ (error_message, ErrorMessages::failed_file_load + "DoesntExist.jpg");

    ASSERT_TRUE(tags.loadHeader(TagsTestCommon::testJpgNon2g(), error_message));

    ASSERT_EQ(tags.orientation(), Tags::ORIENTATION_EXIF_TOPLEFT);
    ASSERT_EQ(tags.imageWidth(), 640);
    ASSERT_EQ(tags.imageHeight(), 480);
    ASSERT_DOUBLE_EQ(tags.fNumber(), 4.7);
    // ASSERT_EQ (tags.make(), "NIKON");
    ASSERT_DOUBLE_EQ(tags.exposureTime(), 1 / 95.70000727320055 * 1000);
    ASSERT_EQ(tags.latitudeRef(), Tags::LATITUDEREF_NORTH);
    ASSERT_DOUBLE_EQ(tags.latitude(), 43.467081666663894);
}

TEST(TagsTest, ParseNon2GTifFile) {
    Tags tags;
    std::string error_message;

    ASSERT_TRUE(tags.loadHeader(TagsTestCommon::testTifNon2g(), error_message));

    ASSERT_EQ(tags.orientation(), Tags::ORIENTATION_EXIF_TOPLEFT);
    ASSERT_EQ(tags.imageWidth(), 15);
    ASSERT_EQ(tags.imageHeight(), 7);
    // ASSERT_EQ (tags.imageDescription(), "Test image");
}

TEST(TagsTest, ParseOld2GTifFile) {
    Tags tags;
    std::string error_message;

    ASSERT_TRUE(tags.loadHeader(TagsTestCommon::testTifOld2g(), error_message));

    ASSERT_EQ(tags.orientation(), Tags::ORIENTATION_EXIF_TOPLEFT);
    ASSERT_EQ(tags.imageWidth(), 2464);
    ASSERT_EQ(tags.imageHeight(), 2056);
    ASSERT_EQ(tags.ppsTime(), 711698604);
    ASSERT_EQ(tags.bitsPerSample().size(), 1);
    ASSERT_EQ(tags.bitsPerSample()[0], 8);
}

TEST(TagsTest, ParseOldVoyisTifFile) {
    Tags tags;
    std::string error_message;

    ASSERT_TRUE(tags.loadHeader(TagsTestCommon::testTifOldVoyis(), error_message));

    ASSERT_EQ(tags.orientation(), Tags::ORIENTATION_EXIF_TOPLEFT);
    ASSERT_EQ(tags.imageWidth(), 4112);
    ASSERT_EQ(tags.imageHeight(), 3008);
    ASSERT_EQ(tags.dateTime(), 1631921861667666);
    ASSERT_EQ(tags.bitsPerSample().size(), 1);
    ASSERT_EQ(tags.bitsPerSample()[0], 16);
}

TEST(TagsTest, Tags_generate_tag_and_reparse) {
    Tags tags;
    TagsTestCommon::setTags(tags);

    std::unique_ptr<unsigned char[], decltype(&std::free)> data{
        static_cast<unsigned char*>(nullptr), std::free};
    unsigned int data_length;
    std::string error_message;

    ASSERT_TRUE(tags.generateHeader(data, data_length, error_message));

    Tags new_tags;
    std::vector<uint8_t> new_header(data.get(), data.get() + data_length / sizeof(unsigned char));

    ASSERT_TRUE(new_tags.loadHeader(new_header, error_message));

    TagsTestCommon::testTags(new_tags);
}

TEST(TagsTest, Clone) {

    Tags tags, cloned_tags;

    tags.imageHeight(1024);
    tags.imageWidth(2048);
    tags.bitsPerSample(std::vector<uint16_t>{8, 8, 8});
    tags.compression(Tags::COMPRESSION_EXIF_JPEG);
    tags.photometricInterpolation(Tags::PHOTOMETRIC_EXIF_RGB);
    tags.imageDescription("Test description!");
    tags.model("Test model!");
    tags.stripOffsets(std::vector<uint32_t>{440});
    tags.samplesPerPixel(3);
    tags.rowsPerStrip(56);
    tags.stripByteCount(std::vector<uint32_t>{1025});
    tags.software("Test software!");
    tags.exposureTime(3.0);
    tags.fNumber(5.0);
    tags.dateTime(1614632629005001);
    tags.subjectDistance(6.0);
    tags.lightSource(Tags::LIGHTSOURCE_BLUELED);
    tags.flash(Tags::FLASH_FIRED);
    tags.focalLength(1.2);
    tags.colourSpace(Tags::COLOURSPACE_ADOBE_RGB);
    tags.flashEnergy(67.0);
    tags.serialNumber("Test serial number");
    tags.lensModel("Test lens model");
    tags.indexOfRefraction(1.345);
    tags.viewportIndex(1.4);
    tags.viewportThickness(0.25);
    tags.viewportDistance(0.26);
    tags.vignetting(true);
    tags.viewportType(Tags::VIEWPORT_DOMED);
    tags.enhancement(Tags::ENHANCEMENT_BASIC_COLOUR);

    const std::vector<uint16_t> pixel_vec = {3600, 3700};
    tags.pixelSize(pixel_vec);

    const std::vector<double> nav_vec = {
        1.0, 2.0, 3.0, 4.0, 5.0, 1.0, 6.0, 7.0, 8.0, 9.0, 1.0, 11.0, 12.0, 13.0, 14.0, 1.0};
    tags.matrixNavToCamera(nav_vec);

    tags.imageNumber(3);
    tags.waterDepth(10.5);
    tags.bayerPattern(Tags::BAYER_BG2BGR);
    tags.frameRate(45.0);

    const std::vector<double> cam_vec = {2000.0, 2001.0, 1036, 738.2};
    tags.cameraMatrix(cam_vec);

    const std::vector<double> dist_vec = {-0.1, 0.2, -0.3, 0.5, -0.5};
    tags.distortion(dist_vec);

    const std::vector<double> pose_vec = {1.0, 2.3, 3.4};
    tags.pose(pose_vec);

    tags.vehicleAltitude(10.87);
    tags.latitudeRef(Tags::LATITUDEREF_SOUTH);
    tags.latitude(83.1245678);
    tags.longitudeRef(Tags::LONGITUDEREF_WEST);
    tags.longitude(43.67890);
    tags.altitudeRef(Tags::ALTITUDEREF_BELOW_SEA_LEVEL);
    tags.altitude(10.765);
    tags.ppsTime(1614632629005011);
    tags.ppsTime(1714632629005021);
    tags.dateTime(1714632629005021);

    cloned_tags = tags.clone();

    ASSERT_EQ(cloned_tags.imageHeight(), tags.imageHeight());
    ASSERT_EQ(cloned_tags.imageWidth(), tags.imageWidth());

    ASSERT_EQ(cloned_tags.bitsPerSample().size(), tags.bitsPerSample().size());
    ASSERT_EQ(cloned_tags.bitsPerSample()[0], tags.bitsPerSample()[0]);
    ASSERT_EQ(cloned_tags.bitsPerSample()[1], tags.bitsPerSample()[1]);
    ASSERT_EQ(cloned_tags.bitsPerSample()[2], tags.bitsPerSample()[2]);

    ASSERT_EQ(cloned_tags.compression(), tags.compression());
    ASSERT_EQ(cloned_tags.photometricInterpolation(), tags.photometricInterpolation());
    ASSERT_EQ(cloned_tags.imageDescription(), tags.imageDescription());
    ASSERT_EQ(cloned_tags.model(), tags.model());
    ASSERT_EQ(cloned_tags.stripOffsets().size(), tags.stripOffsets().size());
    ASSERT_EQ(cloned_tags.stripOffsets()[0], tags.stripOffsets()[0]);
    ASSERT_EQ(cloned_tags.samplesPerPixel(), tags.samplesPerPixel());
    ASSERT_EQ(cloned_tags.rowsPerStrip(), tags.rowsPerStrip());

    ASSERT_EQ(cloned_tags.stripByteCount().size(), tags.stripByteCount().size());
    ASSERT_EQ(cloned_tags.stripByteCount()[0], tags.stripByteCount()[0]);

    ASSERT_EQ(cloned_tags.software(), tags.software());
    ASSERT_EQ(cloned_tags.exposureTime(), tags.exposureTime());
    ASSERT_EQ(cloned_tags.fNumber(), tags.fNumber());
    ASSERT_EQ(cloned_tags.dateTime(), tags.dateTime());
    ASSERT_EQ(cloned_tags.subjectDistance(), tags.subjectDistance());
    ASSERT_EQ(cloned_tags.lightSource(), tags.lightSource());
    ASSERT_EQ(cloned_tags.flash(), tags.flash());
    ASSERT_EQ(cloned_tags.focalLength(), tags.focalLength());
    ASSERT_EQ(cloned_tags.colourSpace(), tags.colourSpace());
    ASSERT_EQ(cloned_tags.flashEnergy(), tags.flashEnergy());
    ASSERT_EQ(cloned_tags.serialNumber(), tags.serialNumber());
    ASSERT_EQ(cloned_tags.lensModel(), tags.lensModel());
    ASSERT_EQ(cloned_tags.indexOfRefraction(), tags.indexOfRefraction());
    ASSERT_EQ(cloned_tags.viewportIndex(), tags.viewportIndex());
    ASSERT_EQ(cloned_tags.viewportThickness(), tags.viewportThickness());
    ASSERT_EQ(cloned_tags.viewportDistance(), tags.viewportDistance());
    ASSERT_EQ(cloned_tags.vignetting(), tags.vignetting());
    ASSERT_EQ(cloned_tags.viewportType(), tags.viewportType());
    ASSERT_EQ(cloned_tags.enhancement(), tags.enhancement());

    ASSERT_EQ(cloned_tags.pixelSize().size(), tags.pixelSize().size());
    ASSERT_EQ(cloned_tags.pixelSize()[0], tags.pixelSize()[0]);
    ASSERT_EQ(cloned_tags.pixelSize()[1], tags.pixelSize()[1]);

    ASSERT_EQ(cloned_tags.matrixNavToCamera().size(), tags.matrixNavToCamera().size());
    ASSERT_EQ(cloned_tags.distortion().size(), tags.distortion().size());
    ASSERT_EQ(cloned_tags.pose().size(), tags.pose().size());

    ASSERT_EQ(cloned_tags.vehicleAltitude(), tags.vehicleAltitude());
    ASSERT_EQ(cloned_tags.latitudeRef(), tags.latitudeRef());
    ASSERT_EQ(cloned_tags.latitude(), tags.latitude());
    ASSERT_EQ(cloned_tags.longitudeRef(), tags.longitudeRef());
    ASSERT_EQ(cloned_tags.longitude(), tags.longitude());
    ASSERT_EQ(cloned_tags.altitudeRef(), tags.altitudeRef());
    ASSERT_EQ(cloned_tags.altitude(), tags.altitude());
    ASSERT_EQ(cloned_tags.ppsTime(), tags.ppsTime());
    ASSERT_EQ(cloned_tags.dateTime(), tags.dateTime());

    // Change some tags and make sure original remains intact
    cloned_tags.imageHeight(4112);
    cloned_tags.imageWidth(3008);
    cloned_tags.compression(Tags::COMPRESSION_EXIF_NONE);
    cloned_tags.imageDescription("New Test description!");
    cloned_tags.model("New Test model!");
    cloned_tags.latitudeRef(Tags::LATITUDEREF_NORTH);
    cloned_tags.latitude(20.12345);
    cloned_tags.longitudeRef(Tags::LONGITUDEREF_EAST);
    cloned_tags.longitude(10.12345);
    cloned_tags.altitudeRef(Tags::ALTITUDEREF_ABOVE_SEA_LEVEL);
    cloned_tags.altitude(5.765);
    cloned_tags.subjectDistance(3.0);

    ASSERT_NE(cloned_tags.imageHeight(), tags.imageHeight());
    ASSERT_NE(cloned_tags.imageWidth(), tags.imageWidth());
    ASSERT_NE(cloned_tags.compression(), tags.compression());
    ASSERT_NE(cloned_tags.imageDescription(), tags.imageDescription());
    ASSERT_NE(cloned_tags.model(), tags.model());
    ASSERT_NE(cloned_tags.latitudeRef(), tags.latitudeRef());
    ASSERT_NE(cloned_tags.latitude(), tags.latitude());
    ASSERT_NE(cloned_tags.longitudeRef(), tags.longitudeRef());
    ASSERT_NE(cloned_tags.longitude(), tags.longitude());
    ASSERT_NE(cloned_tags.altitudeRef(), tags.altitudeRef());
    ASSERT_NE(cloned_tags.altitude(), tags.altitude());
    ASSERT_NE(cloned_tags.subjectDistance(), tags.subjectDistance());
}

} // namespace tags
} // namespace tg