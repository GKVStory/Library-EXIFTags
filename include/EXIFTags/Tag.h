/**
 * Tag.h
 * 
 * Copyright 2G Robotics Inc., 2021
 * 
 * A set of classes derived from the abstract base class tag that represent different sort of EXIF tags.
 * 
 * Only IFD 0 are supported (main image, no thumbnail support).
 * 
 */

#include <libexif/exif-data.h>
#include <vector>
#include <string>
#include <memory>

#include "EXIFTags/TagConstants.h"

/* byte order to use in the EXIF block */
#define FILE_BYTE_ORDER EXIF_BYTE_ORDER_INTEL

namespace tg {
namespace tags {

/**
 * Base Tag type
 */
class Tag {
public:
    //Tag Destructor
    virtual ~Tag() {};

    /**
     * @brief Factory method to create only supported tags.
     * @param tag supported tag type, defined in TagConstants.h
     * @return unique pointer to tag type.
     */
    static std::unique_ptr<Tag> tagFactory (const SupportedTags &  tag);

    /**
     * @brief Load data from a block of memory from the header.
     * @param pointer to header data (lifetime handled by caller)
     * @param length of header buffer
     * @param error message string returned by reference.
     * @return unique pointer to tags containing data that's in the header file. If null, there was a problem.
     */
    static std::unique_ptr<Tag> loadFromHeader(const SupportedTags &  tag, const uint8_t * header, size_t header_len, std::string & error_message);


    /** 
     * @brief Given an ExifData structure, add this tag to the structure Overridden in base class.
     * @param pointer to ExifData structure
     */
    virtual void setTag (ExifData *exif) const = 0;

protected:
    //Tag constructor //stores a reference to the tag structure. This is a reference, 
    //and should only reference something with the same lifetime as the program, like the 
    //TAG_INFO vector in TagConstants.h
    Tag( const TagInfo & tag_info ) : 
        m_tag_info(tag_info)
        {};

private:
    TagInfo m_tag_info;

};

/**
 * Tag for uint32 types
 */
class Tag_UINT32 : public Tag {
public:
    virtual void setTag (ExifData *exif) const override {

    }

    virtual bool getTag (ExifData * data) {

        return false;
    }

    uint32_t getData () const {
        return m_data;
    };
    void setData(const uint32_t & data) {
        m_data = data;
    };

    Tag_UINT32( const TagInfo & tag_info ) : Tag(tag_info) {}

private:
    
    uint32_t m_data;
};

/**
 * Tag for uint16 types
 */
class Tag_UINT16 : public Tag {
public:
    virtual void setTag (ExifData *exif) const override {

    }

    virtual bool getTag (ExifData * data) {

        return false;
    }

    uint16_t getData () const {
        return m_data;
    };
    void setData(const uint16_t & data) {
        m_data = data;
    };

    Tag_UINT16( const TagInfo & tag_info ) : Tag(tag_info) {}

private:

    uint16_t m_data;
};

/**
 * Tag for uint16 types
 */
class Tag_UINT8 : public Tag {
public:
    virtual void setTag (ExifData *exif) const override {

    }

    virtual bool getTag (ExifData * data) {

        return false;
    }

    uint8_t getData () const {
        return m_data;
    };
    void setData(const uint8_t & data) {
        m_data = data;
    };

    Tag_UINT8( const TagInfo & tag_info ) : Tag(tag_info) {}

private:

    uint8_t m_data;
};

/**
 * Tag for double types
 */
class Tag_DOUBLE : public Tag {
public:
    virtual void setTag (ExifData *exif) const override {

    }

    virtual bool getTag (ExifData * data) {

        return false;
    }

    double getData () const {
        return m_data;
    };
    void setData(const double & data) {
        m_data = data;
    };

    Tag_DOUBLE( const TagInfo & tag_info ) : Tag(tag_info) {}

private:

    double m_data;
};

/**
 * Tag for string types
 */
class Tag_STRING : public Tag {
public:
    virtual void setTag (ExifData *exif) const override {

    }

    virtual bool getTag (ExifData * data) {

        return false;
    }

    std::string getData () const {
        return m_data;
    };
    void setData(const std::string & data) {
        m_data = data;
    };

    Tag_STRING( const TagInfo & tag_info ) : Tag(tag_info) {}

private:

    std::string m_data;
};

/**
 * Tag for uint16 vector/array types
 */
class Tag_UINT16_ARRAY : public Tag {
public:
    virtual void setTag (ExifData *exif) const override {

    }

    virtual bool getTag (ExifData * data) {

        return false;
    }

    std::vector <uint16_t> getData () const {
        return m_data;
    };
    void setData(const std::vector <uint16_t> & data) {
        m_data = data;
    };

    Tag_UINT16_ARRAY( const TagInfo & tag_info ) : Tag(tag_info) {}

private:

    std::vector <uint16_t> m_data;
};

/**
 * Tag for double array types
 */
class Tag_DOUBLE_ARRAY : public Tag{
public:
    virtual void setTag (ExifData *exif) const override {

    }

    virtual bool getTag (ExifData * data) {

        return false;
    }
    std::vector <double> getData () const {
        return m_data;
    };
    void setData(const std::vector <double> & data) {
        m_data = data;
    };

    Tag_DOUBLE_ARRAY( const TagInfo & tag_info ) : Tag(tag_info) {}

private:

    std::vector <double> m_data;
};


}
}