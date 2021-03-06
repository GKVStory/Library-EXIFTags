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

    unsigned char header_bytes[2] = {0, 0};
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    if (size < 2) {
        error_message = ErrorMessages::file_too_small + filename;
        return false;
    }
    if (!file.read(reinterpret_cast <char*>(header_bytes), 2 )) {
        error_message = ErrorMessages::failed_file_load + filename;
        return false;
    }
    file.seekg(0, std::ios::beg);


    if ((header_bytes[0] == JPEGHeaderStart[0]) && (header_bytes[1] == JPEGHeaderStart[1])) { //header is from a jpeg file, read in file as is.
        std::streamsize read_size = size > MAX_READ_SIZE ? MAX_READ_SIZE : size;
        image_header_data.resize(static_cast<unsigned int>(read_size));
        if (!file.read(reinterpret_cast<char *>(image_header_data.data()), read_size )) {
            error_message = ErrorMessages::failed_file_load + filename;
            return false;
        }
    } else { //header is tiff (or something else), prepend jpeg delimiter required by exiflib to find tag structure.
        //The tiff header can be located at the back of the file, need to follow the original file offset to find it and then replace the offset in the data copied out of the file.
        
        if (processTiff (filename, error_message)) {
            error_message = failed_file_load + filename
            return false;
        }
        
    }
    
    return true;
}

bool ImageHandler::processTiff (std::vector<uint8_t> & data, std::string & error_message) {
    std::streamsize read_size = size > MAX_READ_SIZE ? MAX_READ_SIZE : size;
    std::streamsize offset = 0;
    image_header_data.resize(static_cast<unsigned int>(read_size+6));
    for (offset = 0; offset < 6; ++offset) {
        image_header_data[i] = static_cast<uint8_t>(ExifHeader[i]);
    }

    //Read in the tiff header and offset in bytes from the head of the file to the data header.
    if (!file.read(reinterpret_cast<char *>(image_header_data.data()+offset), 8)) {
        error_message = ErrorMessages::failed_file_load + filename;
        return false;
    }
    offset += 8

    bool is_le = true;
    if (memcmp (image_header_data.data()+6, TIFFHeaderMotorola, 4) == 0) {
        //headers match, 
        is_le = false;
    }

    //Pull out the offset to the memory address of the data. 
    uint32_t file_offset = 0;
 
       
}