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

class ImageHandler { 
public:

    /**
     * @brief Given a file, load the included tags.
     * @param[in] image_header_data, vector of bytes containing at a minimum the image header data.
     * @param[in/out] image_header_data, vector of bytes containing at a minimum the image header data.
     * @param[out] error emssage returned by reference in case of a failure.
     * @return bool was the load successful?
     */
    static bool loadHeader(const std::string & filename, std::vector <uint8_t> & image_header_data, std::string & error_message);

private:

    static bool processTiff (std::vector <uint8_t> & data, std::string & error_message);
    /*! Magic number for TIFF files */
    static const unsigned char ExifHeader[6];
    static const unsigned char TIFFHeaderMotorola[4];
    static const unsigned char TIFFHeaderIntel[4];
    static const unsigned char JPEGHeaderStart[2];
    
};

} //tags
} //tg
