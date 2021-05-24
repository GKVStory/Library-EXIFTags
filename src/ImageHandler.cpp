// ImageHandler.cpp
// Copyright 2G Robotics Inc., 2021
#include "EXIFTags/ImageHandler.h"
#include "EXIFTags/TagConstants.h"
#include "EXIFTags/Tags.h"

#include <fstream>
#include <algorithm>
#include <memory>
#include <cassert>

using namespace tg;
using namespace tags;

#define MAX_READ_SIZE 64 * 1024

const unsigned char ImageHandler::ExifHeader[6] = {0x45, 0x78, 0x69, 0x66, 0x00, 0x00};
const unsigned char ImageHandler::TIFFHeaderMotorola[4] = {'M', 'M', 0, 42};
const unsigned char ImageHandler::TIFFHeaderIntel[4] = {'I', 'I', 42, 0};
const unsigned char ImageHandler::JPEGHeaderStart[2] = {0xff, 0xd8};
const unsigned char ImageHandler::APP0[2] = {0xff, 0xe0};
const unsigned char ImageHandler::APP1[2] = {0xff, 0xe1};
const unsigned char ImageHandler::STRIP_OFFSET[8] = {
    0x11, 0x01, 0x07, 0x00, 0x04, 0x00, 0x00, 0x00};
const unsigned char ImageHandler::STRIP_OFFSET_ARRAY[4] = {0x11, 0x01, 0x04, 0x00};
const unsigned char ImageHandler::STRIP_SIZE_ARRAY[4] = {0x17, 0x01, 0x03, 0x00};
const unsigned char ImageHandler::OFFSET_LENGTH[8] = {
    0x17, 0x01, 0x07, 0x00, 0x04, 0x00, 0x00, 0x00};
const unsigned char ImageHandler::BITS_PER_SAMPLE[4] = {0x02, 0x01, 0x07, 0x00};
const size_t ImageHandler::HEADER_SIZE = 8;

bool ImageHandler::loadHeader(const std::string& filename,
                              std::vector<uint8_t>& image_header_data,
                              std::string& error_message) {

    image_header_data.clear();
    std::vector <uint8_t> temp_header;
    temp_header.resize(64);
    image_header_data.resize(MAX_READ_SIZE);

    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    if (size < 64) {
        error_message = ErrorMessages::file_too_small + filename;
        return false;
    }

    if (!file.read(reinterpret_cast<char*>(temp_header.data()), 64)) {
        error_message = ErrorMessages::failed_file_load + filename;
        return false;
    }

    bool is_LE = false;
    auto exif_start = std::search(temp_header.begin(),
                                  temp_header.end(),
                                  std::begin(TIFFHeaderMotorola),
                                  std::end(TIFFHeaderMotorola));

    if (exif_start == temp_header.end()) {
        is_LE = true;
        exif_start = std::search(temp_header.begin(),
                                  temp_header.end(),
                                  std::begin(TIFFHeaderIntel),
                                  std::end(TIFFHeaderIntel));
        if (exif_start == temp_header.end()) { 
            error_message = ErrorMessages::invalid_header_data;
            return false;
        }
    }
    auto exif_index = std::distance( temp_header.begin(), exif_start );

    file.seekg(exif_index, std::ios::beg);

    //Read in the first 8 bytes of the header, and find the offset to the start of the header.
    if (!file.read(reinterpret_cast<char*>(image_header_data.data()), HEADER_SIZE)) {
        error_message = ErrorMessages::failed_file_load + filename;
        return false;
    }

    size_t offset;
    if (is_LE) {
        offset = image_header_data[4] +
                    (image_header_data[5] << 8) +
                    (image_header_data[6] << 16) +
                    (image_header_data[7] << 24);
    
        image_header_data[4] = 8;
        image_header_data[5] = 0;
        image_header_data[6] = 0;
        image_header_data[7] = 0;
    } else {
        offset = image_header_data[7] +
                    (image_header_data[6] << 8) +
                    (image_header_data[5] << 16) +
                    (image_header_data[4] << 24);
    
        image_header_data[7] = 8;
        image_header_data[6] = 0;
        image_header_data[5] = 0;
        image_header_data[4] = 0;
    }

    file.seekg(exif_index + offset, std::ios::beg);
    size_t read_size = (int(size) - int(exif_index + offset + 65535 - 8) > 0) ? 65535-8 : size - (exif_index + offset);
    if (!file.read(reinterpret_cast<char*>(image_header_data.data()+8), read_size)) {
        error_message = ErrorMessages::failed_file_load + filename;
        return false;
    }

    return true;
}

bool ImageHandler::tagJpeg(const Tags& exif_tags,
                           const std::vector<uint8_t>& encoded_image,
                           std::vector<uint8_t>& output_image,
                           std::string& error_message) {

    if (encoded_image.size() < static_cast<size_t>(Constants::MIN_IMAGE_SIZE)) {
        error_message = ErrorMessages::image_size_too_small;
        return false;
    }

    if (encoded_image[0] != JPEGHeaderStart[0] && encoded_image[1] != JPEGHeaderStart[1]) {
        error_message = ErrorMessages::not_a_jpeg;
        return false;
    }

    auto start_header_offset =
        std::search(encoded_image.begin(), encoded_image.end(), std::begin(APP0), std::end(APP0));
    if (start_header_offset == encoded_image.end()) {
        error_message = ErrorMessages::not_a_jpeg;
        return false;
    }
    start_header_offset += 2;
    uint16_t app0_offset = ((*start_header_offset) << 8) + *(++start_header_offset);
    start_header_offset += app0_offset - 1; // move to end of APP0

    auto APP1_header_offset =
        std::search(encoded_image.begin(), encoded_image.end(), std::begin(APP1), std::end(APP1));
    auto APP1_header_end = APP1_header_offset;
    if (APP1_header_offset == encoded_image.end()) { // Skip the header app1 note (replace with
                                                     // ours)
        APP1_header_offset = start_header_offset;
        APP1_header_end = start_header_offset;
    } else {
        APP1_header_offset += 2;
        uint16_t APP1_data_size = ((*APP1_header_offset) << 8) + *(++APP1_header_offset);
        APP1_header_offset -= 3;
        APP1_header_end = APP1_header_offset + APP1_data_size + 2;
    }

    std::unique_ptr<unsigned char[], decltype(&std::free)> header_data{
        static_cast<unsigned char*>(nullptr), std::free};
    unsigned int header_length;
    if (!exif_tags.generateHeader(header_data, header_length, error_message)) {
        return false;
    }

    output_image.clear();
    output_image.reserve(encoded_image.size() + header_length);

    for (auto it = encoded_image.begin(); it != APP1_header_offset; ++it) {
        output_image.push_back(*it);
    }
    for (size_t i = 0; i < 2; ++i) {
        output_image.push_back(static_cast<uint8_t>(APP1[i]));
    }
    output_image.push_back(header_length >> 8);
    output_image.push_back(header_length & 0x00FF);
    for (size_t i = 0; i < header_length; ++i) {
        output_image.push_back(static_cast<uint8_t>(header_data[i]));
    }
    for (auto it = APP1_header_end; it != encoded_image.end(); ++it) {
        output_image.push_back(*it);
    }

    return true;
}

bool ImageHandler::tagTiff(Tags& exif_tags,
                           const std::vector<uint8_t>& encoded_image,
                           std::vector<uint8_t>& output_image,
                           std::string& error_message) {

    if (encoded_image.size() < static_cast<size_t>(Constants::MIN_IMAGE_SIZE)) {
        error_message = ErrorMessages::image_size_too_small;
        return false;
    }

    Tags orig_tags;
    if (!orig_tags.loadHeader(encoded_image, error_message)) {
        return false;
    }

    // copy essential fields from the original header to the new header, including the image data.
    exif_tags.photometricInterpolation(orig_tags.photometricInterpolation());
    exif_tags.samplesPerPixel(orig_tags.samplesPerPixel());
    exif_tags.compression(orig_tags.compression());
    exif_tags.imageHeight(orig_tags.imageHeight());
    exif_tags.bitsPerSample(orig_tags.bitsPerSample());
    exif_tags.imageWidth(orig_tags.imageWidth());
    exif_tags.colourSpace(orig_tags.colourSpace());
    // exif_tags.sampleFormat(orig_tags.sampleFormat());
    // exif_tags.predictor(orig_tags.predictor());

    std::vector<uint32_t> offsets = orig_tags.stripOffsets();
    std::vector<uint32_t> strip_bytes = orig_tags.stripByteCount();

    if (strip_bytes.size() == 0 || offsets.size() == 0 ||
        offsets.size() != strip_bytes.size()) { // Images produced in OpenCV cause problems. The
                                                // following code works around the loading problem.

        size_t memory_block_size = strip_bytes.size();
        // Also need to check if the strip offsets are 16 bit. OpenCV will do this depending on
        // settings. If so, we have to double the number of points in teh array,.
        if (strip_bytes[0] >= encoded_image.size()) {

            memory_block_size = memory_block_size * 2;
            strip_bytes.clear();
            strip_bytes.reserve(memory_block_size);

            //  Find the offset of the 0th IFD
            uint32_t offset_of_IFD = encoded_image[4] + (encoded_image[5] << 8) +
                                     (encoded_image[6] << 16) + (encoded_image[7] << 24);

            auto strip_size_tag_start = std::search(encoded_image.begin() + offset_of_IFD,
                                                    encoded_image.end(),
                                                    std::begin(STRIP_SIZE_ARRAY),
                                                    std::end(STRIP_SIZE_ARRAY));
            if (strip_size_tag_start == encoded_image.end()) {
                error_message = ErrorMessages::tiff_header_encoding_failed;
                return false;
            }

            strip_size_tag_start += 8;

            uint32_t offset_to_size = *strip_size_tag_start + (*(strip_size_tag_start + 1) << 8) +
                                      (*(strip_size_tag_start + 2) << 16) +
                                      (*(strip_size_tag_start + 3) << 24);

            for (size_t i = 0; i < memory_block_size; ++i) {
                strip_bytes.push_back(encoded_image[offset_to_size + 2 * i] +
                                      (encoded_image[offset_to_size + 2 * i + 1] << 8));
            }
        }

        // This is a limitation of libexif that needs to be worked around.
        auto strip_offset_tag_start = std::search(encoded_image.begin(),
                                                  encoded_image.end(),
                                                  std::begin(STRIP_OFFSET_ARRAY),
                                                  std::end(STRIP_OFFSET_ARRAY));
        if (strip_offset_tag_start == encoded_image.end()) {
            error_message = ErrorMessages::tiff_header_encoding_failed;
            return false;
        }

        strip_offset_tag_start += 8;

        uint32_t offset_to_offset = *strip_offset_tag_start + (*(strip_offset_tag_start + 1) << 8) +
                                    (*(strip_offset_tag_start + 2) << 16) +
                                    (*(strip_offset_tag_start + 3) << 24);

        offsets.clear();
        offsets.reserve(memory_block_size);

        for (size_t i = 0; i < memory_block_size; ++i) {
            offsets.push_back(encoded_image[offset_to_offset + 4 * i] +
                              (encoded_image[offset_to_offset + 4 * i + 1] << 8) +
                              (encoded_image[offset_to_offset + 4 * i + 2] << 16) +
                              (encoded_image[offset_to_offset + 4 * i + 3] << 24));
        }
    }

    if (offsets.size() != strip_bytes.size()) {
        error_message = ErrorMessages::invalid_image_data;
        return false;
    }

    if (strip_bytes.size() == 0) {
        error_message = ErrorMessages::no_image_data;
        return false;
    }

    uint32_t final_row_size(0);
    std::vector<uint8_t> image_data;
    image_data.reserve(encoded_image.size());
    for (size_t i = 0; i < offsets.size(); ++i) {
        auto start_block(encoded_image.begin() + offsets[i]);
        auto end_block(start_block + strip_bytes[i]);
        image_data.insert(image_data.end(), start_block, end_block);
        final_row_size += strip_bytes[i];
    }

    // need to sync up rows per strip, strip offset and strip byte count. If we only support saving
    // 2g images, then we only need the last two.
    exif_tags.stripByteCount(std::vector<uint32_t>{final_row_size});
    exif_tags.rowsPerStrip(orig_tags.imageWidth());
    exif_tags.stripOffsets(std::vector<uint32_t>{0x0B0E0E0F});

    unsigned int header_length;
    std::unique_ptr<unsigned char[], decltype(&std::free)> header_data{
        static_cast<unsigned char*>(nullptr), std::free};
    if (!exif_tags.generateHeader(header_data, header_length, error_message)) {
        return false;
    }

    if (header_length < sizeof(ExifHeader)) {
        error_message = ErrorMessages::tiff_header_encoding_failed;
        return false;
    }

    output_image.clear();
    output_image.reserve(header_length + final_row_size);
    for (unsigned int i = sizeof(ExifHeader); i < header_length; ++i) {
        output_image.push_back(header_data[i]);
    }
    output_image.insert(output_image.end(), image_data.begin(), image_data.end());

    // change the header offset value to the new position and format. This is faster to do manually
    // than loading the header twice, and works around a problem with the exif tags C library.
    std::vector<uint8_t>::iterator strip_offset_tag_start = std::search(
        output_image.begin(), output_image.end(), std::begin(STRIP_OFFSET), std::end(STRIP_OFFSET));
    if (strip_offset_tag_start == output_image.end()) {
        error_message = ErrorMessages::tiff_header_encoding_failed;
        return false;
    }

    auto data_offset = header_length - 6; // This is suspect, 6 is a magic number, fear it.
    strip_offset_tag_start += 2;
    *strip_offset_tag_start = 0x04;
    strip_offset_tag_start += 2;
    *strip_offset_tag_start = 0x01;
    strip_offset_tag_start += 4;
    *strip_offset_tag_start = data_offset & 0xff;
    ++strip_offset_tag_start;
    *strip_offset_tag_start = (data_offset & 0xff00) >> 8;
    ++strip_offset_tag_start;
    *strip_offset_tag_start = (data_offset & 0xff0000) >> 16;
    ++strip_offset_tag_start;
    *strip_offset_tag_start = (data_offset & 0xff0000000) >> 24;

    std::vector<uint8_t>::iterator data_length_tag_start = std::search(output_image.begin(),
                                                                       output_image.end(),
                                                                       std::begin(OFFSET_LENGTH),
                                                                       std::end(OFFSET_LENGTH));
    if (data_length_tag_start == output_image.end()) {
        error_message = ErrorMessages::tiff_header_encoding_failed;
        return false;
    }

    data_length_tag_start += 2;
    *data_length_tag_start = 0x04;
    data_length_tag_start += 2;
    *data_length_tag_start = 0x01;

    std::vector<uint8_t>::iterator bits_sample_tag_start = std::search(output_image.begin(),
                                                                       output_image.end(),
                                                                       std::begin(BITS_PER_SAMPLE),
                                                                       std::end(BITS_PER_SAMPLE));
    if (bits_sample_tag_start == output_image.end()) {
        error_message = ErrorMessages::tiff_header_encoding_failed;
        return false;
    }

    bits_sample_tag_start += 2;
    *bits_sample_tag_start = 0x03;
    bits_sample_tag_start += 2;
    *bits_sample_tag_start = *bits_sample_tag_start / 2;
    return true;
}