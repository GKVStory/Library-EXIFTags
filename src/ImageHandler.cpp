//ImageHandler.cpp
//Copyright 2G Robotics Inc., 2021
#include "EXIFTags/ImageHandler.h"
#include "EXIFTags/TagConstants.h"
#include "EXIFTags/Tags.h"

#include <fstream>
#include <algorithm>
#include <memory>

using namespace tg;
using namespace tags;

#define MAX_READ_SIZE 64*1024

const unsigned char ImageHandler::ExifHeader[6] = {0x45, 0x78, 0x69, 0x66, 0x00, 0x00};
const unsigned char ImageHandler::TIFFHeaderMotorola[4] = {'M', 'M', 0, 42};
const unsigned char ImageHandler::TIFFHeaderIntel[4] = {'I', 'I', 42, 0};
const unsigned char ImageHandler::JPEGHeaderStart[2] = {0xff, 0xd8};
const unsigned char ImageHandler::APP1[2] = {0xff, 0xe1};

bool ImageHandler::loadHeader(const std::string & filename, std::vector <uint8_t> & image_header_data, std::string & error_message){ 

    image_header_data.clear();
    image_header_data.reserve (6 + MAX_READ_SIZE);

    unsigned char header_bytes[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    if (size < 8) {
        error_message = ErrorMessages::file_too_small + filename;
        return false;
    }
    if (!file.read(reinterpret_cast <char*>(header_bytes), 8 )) {
        error_message = ErrorMessages::failed_file_load + filename;
        return false;
    }
    file.seekg(0, std::ios::beg);

    image_header_data.resize(static_cast<unsigned int>(size));
    if (!file.read(reinterpret_cast<char *>(image_header_data.data()), size )) {
        error_message = ErrorMessages::failed_file_load + filename;
        return false;
    }
      
    return true;
}

bool ImageHandler::tagJpeg (const Tags & exif_tags, const std::vector <uint8_t> & encoded_image, std::vector<uint8_t> & output_image, std::string & error_message) {

    if (encoded_image.size() < static_cast<size_t>(Constants::MIN_IMAGE_SIZE)) {
        error_message = ErrorMessages::image_size_too_small;
        return false;
    }

    if (encoded_image[0] != JPEGHeaderStart[0] && encoded_image[1] != JPEGHeaderStart[1] ) {
        error_message = ErrorMessages::not_a_jpeg;
        return false;
    }

    auto start_header_offset = std::search (encoded_image.begin(), encoded_image.end(), std::begin(APP1), std::end(APP1));
    uint16_t data_size = 0;
    if (start_header_offset == encoded_image.end()) { //No app1 note
        start_header_offset = encoded_image.begin();
        start_header_offset += 2; //skip the header
    } else {
        data_size = ((*start_header_offset) << 8) + *(++start_header_offset);
    }

    auto end_header_offset = start_header_offset + data_size;


    std::unique_ptr <unsigned char[], decltype(&std::free)> header_data {static_cast<unsigned char *>(nullptr), std::free};
    unsigned int header_length;
    if ( !exif_tags.generateHeader( header_data, header_length, error_message )) {
        return false;
    }

    output_image.clear();
    output_image.reserve( encoded_image.size() + header_length );

    for (auto it = encoded_image.begin(); it != start_header_offset; ++it) {
        output_image.push_back(*it);
    }
    for (size_t i = 0; i < header_length; ++i) {
        output_image.push_back (static_cast<uint8_t>(header_data[i]));
    }
    for (auto it = end_header_offset; it != encoded_image.end(); ++it) {
        output_image.push_back (*it);
    }
    
    return true;
}

bool ImageHandler::tagTiff(const Tags & exif_tags, const std::vector <uint8_t> & encoded_image, std::vector<uint8_t> & output_image, std::string & error_message) {

    if (encoded_image.size() < static_cast<size_t>(Constants::MIN_IMAGE_SIZE)) {
        error_message = ErrorMessages::image_size_too_small;
        return false;
    }


    error_message = "Not implemented.";
    return false;
}