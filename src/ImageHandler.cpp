//ImageHandler.cpp
//Copyright 2G Robotics Inc., 2021
#include "EXIFTags/ImageHandler.h"
#include "EXIFTags/TagConstants.h"

#include <fstream>

using namespace tg;
using namespace tags;

#define MAX_READ_SIZE 64*1024

const unsigned char ImageHandler::ExifHeader[6] = {0x45, 0x78, 0x69, 0x66, 0x00, 0x00};
const unsigned char ImageHandler::TIFFHeaderMotorola[4] = {'M', 'M', 0, 42};
const unsigned char ImageHandler::TIFFHeaderIntel[4] = {'I', 'I', 42, 0};
const unsigned char ImageHandler::JPEGHeaderStart[2] = {0xff, 0xd8};

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

