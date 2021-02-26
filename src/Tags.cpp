//Tags.cpp
//Copyright 2G Robotics Inc., 2021

#include "EXIFTags/Tags.h"

using namespace tg;
using namespace tags;

Tags::Tags() {
    m_tags.reserve( Constants::LENGTH_SUPPORTED_TAGS );
    for (auto i = 0; i < Constants::LENGTH_SUPPORTED_TAGS; ++i) {
        m_tags.push_back(Tag::tagFactory(static_cast<Constants::SupportedTags>(i)));
    }

    //Set the default, non-user accessible tags
    dynamic_cast<Tag_UINT32*>(m_tags[Constants::SUBFILE_TYPE].get())->setData(FULL_RESOLUTION_IMAGE);
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::COMPRESSION].get())->setData(COMPRESSION_NONE);
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::PHOTOMETRIC_INTERPOLATION].get())->setData(PHOTOMETRIC_MINISBLACK);
    dynamic_cast<Tag_STRING*>(m_tags[Constants::MAKE].get())->setData(Constants::DEFAULT_MAKE);
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::PHOTOMETRIC_INTERPOLATION].get())->setData(PHOTOMETRIC_MINISBLACK);
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::ORIENTATION].get())->setData(ORIENTATION_TOPLEFT);
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::SAMPLES_PER_PIXEL].get())->setData(1);
}

Tags::~Tags() {}

bool Tags::loadHeader(const std::vector <uint8_t> & image_header_data, std::string & error_message) {
    return false;
}

bool Tags::generateHeader(std::vector <uint8_t> & image_header_data, std::string & error_message) {
    return false;
}

Tags::SubfileTypes Tags::subfileType () const {
    return static_cast<SubfileTypes>(dynamic_cast<Tag_UINT32*>(m_tags[Constants::SUBFILE_TYPE].get())->getData());
}

uint32_t Tags::imageWidth() const {
    return dynamic_cast<Tag_UINT32*>(m_tags[Constants::IMAGE_HEIGHT].get())->getData();
}
void Tags::imageWidth(uint32_t width) {
    dynamic_cast<Tag_UINT32*>(m_tags[Constants::IMAGE_HEIGHT].get())->setData(width);
}

uint32_t Tags::imageHeight() const {
    return dynamic_cast<Tag_UINT32*>(m_tags[Constants::IMAGE_WIDTH].get())->getData();
}
void Tags::imageHeight(uint32_t height) {
    dynamic_cast<Tag_UINT32*>(m_tags[Constants::IMAGE_WIDTH].get())->setData(height);
}

Tags::CompressionType Tags::compression() const {
    return static_cast<CompressionType>(dynamic_cast<Tag_UINT16*>(m_tags[Constants::COMPRESSION].get())->getData());
}
void Tags::compression (Tags::CompressionType compression) {
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::COMPRESSION].get())->setData(compression);
}

Tags::PhotometricInterpolationType Tags::photometricInterpolation() const {
    return static_cast<PhotometricInterpolationType>(dynamic_cast<Tag_UINT16*>(m_tags[Constants::PHOTOMETRIC_INTERPOLATION].get())->getData());
}
void Tags::photometricInterpolation (Tags::PhotometricInterpolationType pi) {
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::PHOTOMETRIC_INTERPOLATION].get())->setData(pi);
}

std::string Tags::imageDescription() const {
    return dynamic_cast<Tag_STRING*>(m_tags[Constants::IMAGE_DESCRIPTION].get())->getData();
}
void Tags::imageDescription(const std::string & desc) {
    dynamic_cast<Tag_STRING*>(m_tags[Constants::IMAGE_DESCRIPTION].get())->setData(desc);
}

std::string Tags::make() const {
    return dynamic_cast<Tag_STRING*>(m_tags[Constants::MAKE].get())->getData();
}

std::string Tags::model() const{
    return dynamic_cast<Tag_STRING*>(m_tags[Constants::MODEL].get())->getData();
}
void Tags::model(const std::string & model) {
    dynamic_cast<Tag_STRING*>(m_tags[Constants::MODEL].get())->setData(model);
}

Tags::OrientationType Tags::orientation() const {
     return static_cast<OrientationType>(dynamic_cast<Tag_UINT16*>(m_tags[Constants::ORIENTATION].get())->getData());
}

uint16_t Tags::samplesPerPixel () const {
    return dynamic_cast<Tag_UINT16*>(m_tags[Constants::SAMPLES_PER_PIXEL].get())->getData();
}
void Tags::samplesPerPixel (uint16_t samples) {
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::SAMPLES_PER_PIXEL].get())->setData(samples);
}