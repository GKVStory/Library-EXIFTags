//Tag.cpp
//Copyright 2G Robotics Inc., 2021
#include "EXIFTags/Tag.h"

namespace tg {
namespace tags {

std::unique_ptr<Tag> Tag::tagFactory (const SupportedTags &  tag) {
    switch (TAG_INFO[tag].data_type) {
        case UINT32:
            return std::make_unique<Tag_UINT32> (TAG_INFO[tag]);
        case UINT16:
            return std::make_unique<Tag_UINT16> (TAG_INFO[tag]);
        case UINT8:
            return std::make_unique<Tag_UINT8> (TAG_INFO[tag]);
        case DOUBLE:
            return std::make_unique<Tag_DOUBLE> (TAG_INFO[tag]);
        case STRING:
            return std::make_unique<Tag_STRING> (TAG_INFO[tag]);
        case UINT16_ARRAY:
            return std::make_unique<Tag_UINT16_ARRAY> (TAG_INFO[tag]);
        case DOUBLE_ARRAY:
            return std::make_unique<Tag_DOUBLE_ARRAY> (TAG_INFO[tag]);
        default:
            return nullptr;
    }
    
}

std::unique_ptr<Tag> Tag::loadFromHeader(const SupportedTags &  tag, const uint8_t * header, size_t header_len, std::string & error_message) {
    TagInfo tag_info = TAG_INFO[tag];
    //TODO do the parsing. Different handling for 2g header versus standard exif.
    return nullptr;
}

} //tags
} //tg