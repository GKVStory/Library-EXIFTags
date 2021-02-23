/**
 * Tag.h
 * 
 * Copyright 2G Robotics Inc., 2021
 * 
 * A set of classes derived from the abstract base class tag that represent different sort of EXIF tags.
 * 
 */

#include <libexif/exif-data.h>

namespace tg {
namespace tags {

class Tag {
public:
    Tag( uint16_t tag_id );
    virtual ~Tag();

    void setTag (ExifData *exif);

private:

    uint16_t m_tag_id;

}

}
}