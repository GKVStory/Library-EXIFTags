//Tag.cpp
//Copyright 2G Robotics Inc., 2021
#include "EXIFTags/Tag.h"

namespace tg {
namespace tags {

std::unique_ptr<Tag> Tag::tagFactory (const Constants::SupportedTags &  tag) {
    switch (Constants::TAG_INFO[tag].data_type) {
        case Constants::UINT32:
            return std::make_unique<Tag_UINT32> (Constants::TAG_INFO[tag]);
        case Constants::UINT16:
            return std::make_unique<Tag_UINT16> (Constants::TAG_INFO[tag]);
        case Constants::UINT8:
            return std::make_unique<Tag_UINT8> (Constants::TAG_INFO[tag]);
        case Constants::DOUBLE:
            return std::make_unique<Tag_DOUBLE> (Constants::TAG_INFO[tag]);
        case Constants::STRING:
            return std::make_unique<Tag_STRING> (Constants::TAG_INFO[tag]);
        case Constants::UINT16_ARRAY:
            return std::make_unique<Tag_UINT16_ARRAY> (Constants::TAG_INFO[tag]);
        case Constants::DOUBLE_ARRAY:
            return std::make_unique<Tag_DOUBLE_ARRAY> (Constants::TAG_INFO[tag]);
        default:
            return nullptr;
    }
    
}

/*std::unique_ptr<Tag> Tag::loadFromHeader(const Constants::SupportedTags &  tag, const uint8_t * header, size_t header_len, std::string & error_message) {
    Constants::TagInfo tag_info = Constants::TAG_INFO[tag];
    //TODO do the parsing. Different handling for 2g header versus standard exif.
    return nullptr;
}*/

} //tags
} //tg