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
const unsigned char ImageHandler::APP0[2] = {0xff, 0xe0};
const unsigned char ImageHandler::APP1[2] = {0xff, 0xe1};
const unsigned char ImageHandler::STRIP_OFFSET[12] = {0x11, 0x01, 0x07, 0x00, 0x04, 0x00, 0x00, 0x00, 0x0F, 0x0E, 0x0E, 0x0B};
const unsigned char ImageHandler::OFFSET_LENGTH[8] = {0x17, 0x01, 0x07, 0x00, 0x04, 0x00, 0x00, 0x00};



bool ImageHandler::loadHeader(const std::string & filename, std::vector <uint8_t> & image_header_data, std::string & error_message){ 

    image_header_data.clear();
    image_header_data.reserve (6 + MAX_READ_SIZE);

    unsigned char header_bytes[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    if (size < 32) {
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

    auto start_header_offset = std::search (encoded_image.begin(), encoded_image.end(), std::begin(APP0), std::end(APP0));
    if (start_header_offset == encoded_image.end()) {
        error_message = ErrorMessages::not_a_jpeg;
        return false;
    }
    start_header_offset += 2;
    uint16_t app0_offset = ((*start_header_offset) << 8) + *(++start_header_offset);
    start_header_offset += app0_offset - 1; // move to end of APP0

    auto APP1_header_offset = std::search (encoded_image.begin(), encoded_image.end(), std::begin(APP1), std::end(APP1));
    auto APP1_header_end = APP1_header_offset;
    if (APP1_header_offset == encoded_image.end()) { //Skip the header app1 note (replace with ours)
        APP1_header_offset = start_header_offset;
        APP1_header_end = start_header_offset;
    } else {
        APP1_header_offset += 2;
        uint16_t APP1_data_size =  ((*APP1_header_offset) << 8) + *(++APP1_header_offset);
        APP1_header_offset -= 3;
        APP1_header_end = APP1_header_offset + APP1_data_size + 2;
    }

    std::unique_ptr <unsigned char[], decltype(&std::free)> header_data {static_cast<unsigned char *>(nullptr), std::free};
    unsigned int header_length;
    if ( !exif_tags.generateHeader( header_data, header_length, error_message )) {
        return false;
    }

    output_image.clear();
    output_image.reserve( encoded_image.size() + header_length );

    for (auto it = encoded_image.begin(); it !=APP1_header_offset; ++it) {
        output_image.push_back(*it);
    }
    for (size_t i = 0; i < 2; ++i) {
        output_image.push_back (static_cast<uint8_t>(APP1[i]));
    }
    output_image.push_back( header_length >> 8);
    output_image.push_back ( header_length & 0x00FF);
    for (size_t i = 0; i < header_length; ++i) {
        output_image.push_back (static_cast<uint8_t>(header_data[i]));
    }
    for (auto it = APP1_header_end; it != encoded_image.end(); ++it) {
        output_image.push_back (*it);
    }
    
    return true;
}

bool ImageHandler::tagTiff(Tags & exif_tags, const std::vector <uint8_t> & encoded_image, std::vector<uint8_t> & output_image, std::string & error_message) {

    if (encoded_image.size() < static_cast<size_t>(Constants::MIN_IMAGE_SIZE)) {
        error_message = ErrorMessages::image_size_too_small;
        return false;
    }

    Tags orig_tags;
    if (!orig_tags.loadHeader(encoded_image, error_message)) {
        return false;
    }

    //copy essential fields from the original header to the new header, including the image data.
    exif_tags.photometricInterpolation(orig_tags.photometricInterpolation());
    exif_tags.samplesPerPixel(orig_tags.samplesPerPixel());
    exif_tags.compression(orig_tags.compression());
    exif_tags.imageHeight(orig_tags.imageHeight());
    exif_tags.samplesPerPixel(orig_tags.samplesPerPixel());
    exif_tags.bitsPerSample(orig_tags.bitsPerSample());
    exif_tags.imageWidth(orig_tags.imageWidth());

    const std::vector<uint32_t> offsets = orig_tags.stripOffsets();
    const std::vector<uint32_t> strip_bytes = orig_tags.stripByteCount();

    if (offsets.size() != strip_bytes.size()) {
        error_message = ErrorMessages::invalid_image_data;
        return false;
    }

    if (offsets.size() == 0) {
        error_message = ErrorMessages::no_image_data;
        return false;
    }

    uint32_t final_row_size (0);
    std::vector<uint8_t> image_data;
    image_data.reserve(encoded_image.size());
    for (int i = 0; i < offsets.size(); ++i) {
        auto start_block (encoded_image.begin() + offsets[i]);
        auto end_block (start_block + strip_bytes[i]);
        image_data.insert (image_data.end(), start_block, end_block);
        final_row_size += strip_bytes[i];
    }

    //need to sync up rows per strip, strip offset and strip byte count. If we only support saving 2g images, then we only need the last two.
    exif_tags.stripByteCount(std::vector<uint32_t>{final_row_size});
    exif_tags.rowsPerStrip(exif_tags.imageHeight());
    exif_tags.stripOffsets (std::vector<uint32_t>{0x0B0E0E0F});


    unsigned int header_length;
    std::unique_ptr <unsigned char[], decltype(&std::free)> header_data {static_cast<unsigned char *>(nullptr), std::free};
    if ( !exif_tags.generateHeader( header_data, header_length, error_message )) {
        return false;
    }

    if (header_length < sizeof(ExifHeader)) {
        error_message = ErrorMessages::tiff_header_encoding_failed;
        return false;
    }

    output_image.clear();
    output_image.reserve(header_length - 6 + final_row_size); //6 is a magic number, fear it.
    for (unsigned int i = sizeof(ExifHeader); i < header_length; ++i) {
        output_image.push_back(header_data[i]);
    }
    output_image.insert(output_image.end(), image_data.begin(), image_data.end());

    //change the header offset value to the new position and format. This is faster to do manually than loading the header twice, and works around a problem with the exif tags C library.
    std::vector<uint8_t>::iterator strip_offset_tag_start = std::search (output_image.begin(), output_image.end(), std::begin(STRIP_OFFSET), std::end(STRIP_OFFSET));
    if (strip_offset_tag_start == output_image.end()) {
        error_message = ErrorMessages::tiff_header_encoding_failed;
        return false;
    }

    auto data_offset = header_length - 6;
    strip_offset_tag_start += 2;
    *strip_offset_tag_start =  0x04;
    strip_offset_tag_start += 2;
    *strip_offset_tag_start =  0x01;
    strip_offset_tag_start += 4;
    *strip_offset_tag_start = data_offset & 0xff;
    ++strip_offset_tag_start;
    *strip_offset_tag_start = (data_offset & 0xff00) >> 8;
    ++strip_offset_tag_start;
    *strip_offset_tag_start = (data_offset & 0xff0000) >> 16;
    ++strip_offset_tag_start;
    *strip_offset_tag_start = (data_offset & 0xff0000000) >> 24;

    std::vector<uint8_t>::iterator data_length_tag_start = std::search (output_image.begin(), output_image.end(), std::begin(OFFSET_LENGTH), std::end(OFFSET_LENGTH));
    if (data_length_tag_start == output_image.end()) {
        error_message = ErrorMessages::tiff_header_encoding_failed;
        return false;
    }

    data_length_tag_start += 2;
    *data_length_tag_start = 0x04;
    data_length_tag_start += 2;
    *data_length_tag_start = 0x01;
    return true;
}