#pragma once
/**
 * TiffLoader.h
 * 
 * Copyright 2G Robotics Inc., 2021
 * 
 * This file adds tiff support to stock libexif
 */
#include <vector>
#include <string>

namespace tg {
namespace tags {

class Tags; //forward declare

class ImageHandler { 
public:

    /**
     * @brief Given a file, load the included tags.
     * @param[in] filename, path of image to load
     * @param[in/out] image_header_data, vector of bytes containing at a minimum the image header data.
     * @param[out] error emssage returned by reference in case of a failure.
     * @return bool was the load successful?
     */
    static bool loadHeader(const std::string & filename, std::vector <uint8_t> & image_header_data, std::string & error_message);

    /**
     * Given a Tags object and an encoded jpeg image, apply the new exif tag object to the encoded image. 
     * @param Tags [in] constant reference to the tag object.
     * @param vector [in] encoded image data with existing header (what is output by cv::imencode)
     * @param vector [out] reference to output image (contains new header and a copy of the original encoded image data)
     * @param string [out] error message string.
     * @return bool was the tagging successful?
     */
    static bool tagJpeg (const Tags & exif_tags, const std::vector <uint8_t> & encoded_image, std::vector<uint8_t> & output_image, std::string & error_message);

    /**
     * Given a Tags object and an encoded tiff image, apply the new exif tag object to the encoded image. 
     * @param Tags [in] reference to the tag object. This value might be adjusted based on the settings in the encoded image
     * @param vector [in] encoded image data with existing header (what is output by cv::imencode or libtiff)
     * @param vector [out] reference to output image (contains new header and a copy of the original encoded image data)
     * @param string [out] error message string.
     * @return bool was the tagging successful?
     */
    static bool tagTiff(Tags & exif_tags, const std::vector <uint8_t> & encoded_image, std::vector<uint8_t> & output_image, std::string & error_message);

    static const unsigned char JPEGHeaderStart[2];

private:

    /*! Magic number for TIFF and JPEG files */
    static const unsigned char ExifHeader[6];
    static const unsigned char TIFFHeaderMotorola[4];
    static const unsigned char TIFFHeaderIntel[4];
    static const unsigned char APP0[2];
    static const unsigned char APP1[2];
    static const unsigned char STRIP_OFFSET[12];
    static const unsigned char OFFSET_LENGTH[8];
};

} //tags
} //tg
