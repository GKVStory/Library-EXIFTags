//TestTags.cpp
//Copyright 2G Robotics Inc., 2021

#include <gtest/gtest.h>
#include "EXIFTags/Tags.h"
#include "EXIFTags/ImageHandler.h"
#include "TestConstants.h"
#include <fstream>
#include <cstdint>
#include <string>

namespace tg {
namespace tags {

/* raw JPEG image data */
const std::vector<unsigned char> image_jpeg = {
  0xff, 0xd8, 0xff, 0xe0, 0x00, 0x10, 0x4a, 0x46, 0x49, 0x46, 0x00, 0x01,
  0x01, 0x01, 0x00, 0x48, 0x00, 0x48, 0x00, 0x00, 0xff, 0xdb, 0x00, 0x43,
  0x00, 0x14, 0x0e, 0x0f, 0x12, 0x0f, 0x0d, 0x14, 0x12, 0x10, 0x12, 0x17,
  0x15, 0x14, 0x18, 0x1e, 0x32, 0x21, 0x1e, 0x1c, 0x1c, 0x1e, 0x3d, 0x2c,
  0x2e, 0x24, 0x32, 0x49, 0x40, 0x4c, 0x4b, 0x47, 0x40, 0x46, 0x45, 0x50,
  0x5a, 0x73, 0x62, 0x50, 0x55, 0x6d, 0x56, 0x45, 0x46, 0x64, 0x88, 0x65,
  0x6d, 0x77, 0x7b, 0x81, 0x82, 0x81, 0x4e, 0x60, 0x8d, 0x97, 0x8c, 0x7d,
  0x96, 0x73, 0x7e, 0x81, 0x7c, 0xff, 0xc0, 0x00, 0x0b, 0x08, 0x00, 0x40,
  0x00, 0x40, 0x01, 0x01, 0x11, 0x00, 0xff, 0xc4, 0x00, 0x1b, 0x00, 0x00,
  0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x05, 0x06, 0x04, 0x03, 0x07, 0x02, 0x01, 0xff,
  0xc4, 0x00, 0x2f, 0x10, 0x00, 0x01, 0x03, 0x03, 0x02, 0x05, 0x03, 0x03,
  0x03, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x11,
  0x00, 0x04, 0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x81, 0x61, 0x71,
  0x91, 0x13, 0x32, 0xa1, 0x14, 0x22, 0xc1, 0x15, 0x23, 0x52, 0xd1, 0xf0,
  0xff, 0xda, 0x00, 0x08, 0x01, 0x01, 0x00, 0x00, 0x3f, 0x00, 0xb3, 0xa2,
  0x8a, 0x28, 0xa2, 0x8a, 0x28, 0xa2, 0x97, 0x64, 0x72, 0xd6, 0x58, 0xd4,
  0x8f, 0xd5, 0x3d, 0xca, 0xa5, 0x7d, 0xa8, 0x4e, 0xaa, 0x3e, 0xb0, 0x3a,
  0x7a, 0x9d, 0x2b, 0x2d, 0x87, 0x13, 0xe3, 0xaf, 0x9f, 0x0c, 0xb6, 0xb5,
  0xb6, 0xe2, 0xb4, 0x48, 0x71, 0x30, 0x14, 0x7b, 0x02, 0x09, 0x14, 0xee,
  0x94, 0x64, 0x38, 0x87, 0x1f, 0x8d, 0x5f, 0xd3, 0x7d, 0xe2, 0xa7, 0x46,
  0xe8, 0x6c, 0x49, 0x1e, 0xfd, 0x07, 0xb1, 0x33, 0x5f, 0xb8, 0xdc, 0xf5,
  0x96, 0x51, 0x7c, 0x96, 0xee, 0x14, 0xba, 0x04, 0xf2, 0x2c, 0x42, 0xa3,
  0xd3, 0x70, 0x7c, 0x1a, 0x6d, 0x45, 0x2c, 0xcd, 0xe5, 0x11, 0x89, 0xb1,
  0x5b, 0xc4, 0x02, 0xe1, 0xd1, 0xb4, 0xcf, 0xdc, 0xa3, 0xfc, 0x0d, 0xcd,
  0x41, 0xde, 0xe3, 0xb2, 0x2e, 0xda, 0x1c, 0xbd, 0xe2, 0x4a, 0x90, 0xe9,
  0x04, 0x95, 0x2b, 0xf7, 0x41, 0xd0, 0x18, 0xe8, 0x36, 0x03, 0xc6, 0x91,
  0x4a, 0x81, 0x20, 0x82, 0x24, 0x10, 0x66, 0x45, 0x5a, 0xdd, 0xf1, 0x32,
  0xff, 0x00, 0xa2, 0x5a, 0x8b, 0x62, 0x57, 0x7d, 0x70, 0x80, 0x93, 0x1a,
  0x94, 0x10, 0x79, 0x4a, 0xa3, 0xb9, 0x20, 0xc0, 0xf3, 0xd3, 0x55, 0x59,
  0x4e, 0x1c, 0x7a, 0xc7, 0x12, 0x9b, 0xc7, 0x94, 0xa7, 0x2e, 0x14, 0xa9,
  0x74, 0x0d, 0x42, 0x01, 0x04, 0xc9, 0x3b, 0x93, 0x30, 0x09, 0xdb, 0x5f,
  0x35, 0x9b, 0x85, 0xad, 0xde, 0x7f, 0x39, 0x6e, 0xa6, 0x82, 0xa1, 0xa5,
  0x73, 0x2d, 0x40, 0x68, 0x13, 0x06, 0x67, 0xdf, 0x6f, 0x35, 0xe9, 0xb4,
  0x52, 0x4c, 0xbe, 0x0c, 0x65, 0x72, 0x16, 0xcf, 0x3a, 0xff, 0x00, 0xf6,
  0x5a, 0xd1, 0x4d, 0x11, 0xa1, 0x13, 0x26, 0x0f, 0x73, 0x00, 0x1f, 0x41,
  0x5b, 0x72, 0xd6, 0xa2, 0xeb, 0x15, 0x73, 0x6e, 0x13, 0xf7, 0x36, 0x42,
  0x40, 0xee, 0x04, 0x8f, 0xc8, 0x15, 0xe4, 0xf5, 0x59, 0xc1, 0x16, 0x0d,
  0x3d, 0x70, 0xed, 0xdb, 0x84, 0x29, 0x6c, 0x40, 0x42, 0x3b, 0x13, 0x3f,
  0xb8, 0xfc, 0x10, 0x3c, 0xd5, 0xc1, 0x00, 0x88, 0x22, 0x41, 0xef, 0x5f,
  0x0d, 0xb4, 0x86, 0x81, 0x0d, 0xa1, 0x28, 0x07, 0x52, 0x12, 0x00, 0x06,
  0xba, 0xd1, 0x45, 0x71, 0xb8, 0xb8, 0x6e, 0xd9, 0x85, 0xbc, 0xf2, 0xb9,
  0x5b, 0x40, 0x25, 0x47, 0xb0, 0xaf, 0x24, 0x79, 0x48, 0x53, 0xce, 0x29,
  0xb4, 0x94, 0xa0, 0xa8, 0x94, 0xa7, 0xb0, 0x27, 0x41, 0xf1, 0x54, 0x9c,
  0x09, 0x71, 0xc9, 0x92, 0x7d, 0x82, 0x74, 0x75, 0xa9, 0x1e, 0xa5, 0x27,
  0xfd, 0x13, 0x55, 0xd9, 0x3c, 0x83, 0x38, 0xbb, 0x45, 0x5c, 0xbf, 0x3c,
  0xa1, 0x40, 0x40, 0x02, 0x54, 0x4f, 0x41, 0x3e, 0x4f, 0x83, 0x5a, 0x2d,
  0xae, 0x1b, 0xba, 0xb7, 0x6d, 0xf6, 0x89, 0x28, 0x71, 0x21, 0x49, 0x24,
  0x41, 0x83, 0xe9, 0x5d, 0xe9, 0x3f, 0x12, 0x64, 0x97, 0x8b, 0xc6, 0x17,
  0x59, 0x03, 0xea, 0xa9, 0x61, 0x08, 0x24, 0x48, 0x04, 0xc9, 0x9f, 0x80,
  0x6a, 0x4b, 0x17, 0xc4, 0xd7, 0xac, 0xdf, 0x36, 0xab, 0xbb, 0x85, 0x39,
  0x6e, 0xb5, 0x00, 0xe0, 0x50, 0x06, 0x01, 0x3a, 0x91, 0x02, 0x44, 0x6f,
  0x02, 0xb7, 0x65, 0xb2, 0x0e, 0x71, 0x25, 0xf3, 0x58, 0xdc, 0x72, 0x8f,
  0xe9, 0xc1, 0xe6, 0x52, 0xe0, 0xc2, 0xa3, 0x72, 0x46, 0xe0, 0x0e, 0x83,
  0xa9, 0xf1, 0x4b, 0x38, 0x9f, 0x16, 0xde, 0x2e, 0xfd, 0xa4, 0x32, 0x92,
  0x1a, 0x5b, 0x40, 0x83, 0xdc, 0x8d, 0x0f, 0x9d, 0x01, 0x3e, 0xf5, 0x83,
  0x17, 0x7a, 0xac, 0x76, 0x41, 0x9b, 0xa4, 0xa7, 0x9b, 0xe9, 0xab, 0x51,
  0x31, 0x20, 0x82, 0x08, 0xf8, 0x34, 0xf9, 0xdb, 0x97, 0xb8, 0xb7, 0x2c,
  0xcd, 0xba, 0x12, 0xa6, 0xec, 0xda, 0x3c, 0xca, 0x13, 0x24, 0x0e, 0xa4,
  0x91, 0xa4, 0x9d, 0x87, 0x69, 0xf7, 0xab, 0x74, 0x21, 0x2d, 0xa1, 0x28,
  0x4a, 0x42, 0x52, 0x90, 0x02, 0x40, 0xd8, 0x01, 0xb0, 0xae, 0x95, 0x87,
  0x29, 0x8f, 0x6f, 0x27, 0x64, 0xe5, 0xb3, 0xc4, 0x80, 0xa8, 0x29, 0x50,
  0xdd, 0x24, 0x6c, 0x47, 0xfd, 0xd4, 0xd4, 0x92, 0x78, 0x1e, 0xef, 0xeb,
  0x42, 0xae, 0x98, 0x0d, 0x7f, 0x90, 0x0a, 0x27, 0xe2, 0x00, 0xfc, 0xd5,
  0x4e, 0x27, 0x11, 0x6d, 0x89, 0x60, 0xb7, 0x6e, 0x92, 0x54, 0xa8, 0xe6,
  0x71, 0x5b, 0xa8, 0xff, 0x00, 0x03, 0xd2, 0xbe, 0x73, 0x38, 0x86, 0x72,
  0xf6, 0xc1, 0xb7, 0x4f, 0x2a, 0xd1, 0x25, 0x0e, 0x01, 0x25, 0x27, 0xae,
  0x9d, 0x41, 0xed, 0xed, 0x52, 0xe8, 0xe0, 0x7b, 0xcf, 0xab, 0x0e, 0x5d,
  0x30, 0x96, 0xe7, 0xee, 0x4f, 0x31, 0x57, 0xc1, 0x00, 0x7e, 0x6a, 0xaf,
  0x15, 0x8b, 0x63, 0x17, 0x6e, 0x1a, 0x61, 0x24, 0x92, 0x65, 0x6b, 0x3b,
  0xa8, 0xf7, 0x3e, 0x9d, 0x87, 0x4a, 0x63, 0x45, 0x14, 0x51, 0x45, 0x14,
  0x51, 0x45, 0x7f, 0xff, 0xd9
};

/* dimensions of image */
const unsigned int image_jpg_x = 64;
const unsigned int image_jpg_y = 64;

/* raw EXIF header data */
static const unsigned char exif_header[] = {
  0xff, 0xd8, 0xff, 0xe1
};
/* length of data in exif_header */
static const unsigned int exif_header_len = sizeof(exif_header);

TEST ( TEST_ImageHandler, TestJPEG) {
    Tags tags;
    
    TagsTestCommon::setTags(tags);
    tags.imageHeight(image_jpg_x);
    tags.imageWidth(image_jpg_y);
    tags.compression(Tags::COMPRESSION_JPEG);
    tags.colourSpace(Tags::COLOURSPACE_sRGB);

    std::vector <uint8_t> out_image;
    std::string error_message;

    ASSERT_TRUE(ImageHandler::tagJpeg(tags, image_jpeg, out_image, error_message));

    FILE* pFile;
    pFile = fopen(TagsTestCommon::jpegOutputFile().c_str(), "wb");      
    fwrite(out_image.data(), 1, out_image.size()*sizeof(unsigned char), pFile);
    fclose(pFile);

    Tags new_tags;
    
    ASSERT_TRUE (new_tags.loadHeader(TagsTestCommon::jpegOutputFile(), error_message));

    ASSERT_EQ(new_tags.subfileType(), Tags::FULL_RESOLUTION_IMAGE);
    ASSERT_EQ(new_tags.imageHeight(), image_jpg_x);
    ASSERT_EQ(new_tags.imageWidth(), image_jpg_y);
    ASSERT_EQ(new_tags.compression(), Tags::COMPRESSION_JPEG);
    ASSERT_EQ(new_tags.photometricInterpolation(), Tags::PHOTOMETRIC_RGB);
    ASSERT_EQ(tags.colourSpace(), Tags::COLOURSPACE_sRGB);
    TagsTestCommon::testTags(new_tags);

}

TEST (TEST_ImageHandler, TestTIFF) {
    Tags tags;
    TagsTestCommon::setTags(tags);

    std::vector <uint8_t> out_image;
    std::string error_message;

    std::vector<uint8_t> image_tiff;

    std::ifstream infile (TagsTestCommon::testTifOld2g(), std::ios::binary | std::ios::ate );
    std::streamsize file_size = infile.tellg();
    infile.seekg(0, std::ios::beg);

    image_tiff.resize(static_cast<unsigned int>(file_size));
    infile.read(reinterpret_cast<char *>(image_tiff.data()), file_size );
    ASSERT_TRUE(ImageHandler::tagTiff(tags, image_tiff, out_image, error_message));

    FILE* pFile;
    pFile = fopen(TagsTestCommon::tiffOutputFile().c_str(), "wb");      
    fwrite(out_image.data(), 1, out_image.size()*sizeof(unsigned char), pFile);
    fclose(pFile);

    Tags new_tags;
    
    ASSERT_TRUE (new_tags.loadHeader(TagsTestCommon::tiffOutputFile(), error_message));

    ASSERT_EQ(new_tags.subfileType(), Tags::FULL_RESOLUTION_IMAGE);
    ASSERT_EQ(new_tags.imageHeight(), 2056);
    ASSERT_EQ(new_tags.imageWidth(), 2464);
    ASSERT_EQ(new_tags.compression(), Tags::COMPRESSION_NONE);
    ASSERT_EQ(new_tags.photometricInterpolation(), Tags::PHOTOMETRIC_MINISBLACK);
    TagsTestCommon::testTags(new_tags);
}

}
}