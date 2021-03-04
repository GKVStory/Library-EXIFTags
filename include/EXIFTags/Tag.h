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
#include <cmath>

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
    static std::unique_ptr<Tag> tagFactory (const Constants::SupportedTags &  tag);

    /**
     * @brief Load data from a block of memory from the header.
     * @param pointer to header data (lifetime handled by caller)
     * @param length of header buffer
     * @param error message string returned by reference.
     * @return unique pointer to tags containing data that's in the header file. If null, there was a problem.
     */
    //static std::unique_ptr<Tag> loadFromHeader(const Constants::SupportedTags &  tag, const uint8_t * header, size_t header_len, std::string & error_message);

    /** 
     * @brief Given an ExifData structure, add this tag to the structure Overridden in base class.
     * @param pointer to ExifData structure
     */
    virtual void setTag (ExifData *exif) const = 0;

    /**
     * Given loaded exif data, extract the tag into the data structure.
     * @param pointer to exif data.
     * @return was the load successful?
     */
    virtual bool getTag (ExifData *exif) = 0;

    /**
     * Check if the tag is a standard type.
     * @return is the tag a standard exif one.
     */
    bool isStandardTag() const {
        return !m_tag_info.custom;
    };

protected:
    //Tag constructor //stores a reference to the tag structure. This is a reference, 
    //and should only reference something with the same lifetime as the program, like the 
    //TAG_INFO vector in TagConstants.h
    Tag( const Constants::TagInfo & tag_info ) : 
        m_tag_info(tag_info)
        {};

    Constants::TagInfo m_tag_info;

};

/**
 * Tag for uint32 types
 */
class Tag_UINT32 : public Tag {
public:
    virtual void setTag (ExifData *exif) const override {

    }

    virtual bool getTag (ExifData * ed) override {
        ExifEntry *entry = exif_content_get_entry(ed->ifd[m_tag_info.ifd], static_cast<ExifTag>(m_tag_info.tag)); //points to exif data, do not delete.
        if (entry) {
            m_data = *(reinterpret_cast<uint32_t *> (entry->data));
        } else {
            return false;
        }
        return true;
    }

    uint32_t getData () const {
        return m_data;
    };
    void setData(const uint32_t & data) {
        m_data = data;
    };

    Tag_UINT32( const Constants::TagInfo & tag_info ) : 
        Tag(tag_info),
        m_data(0) {}

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

    virtual bool getTag (ExifData * ed) {
        ExifEntry *entry = exif_content_get_entry(ed->ifd[m_tag_info.ifd], static_cast<ExifTag>(m_tag_info.tag)); //points to exif data, do not delete.
        if (entry) {
            m_data = *(reinterpret_cast<uint16_t *> (entry->data));
        } else {
            return false;
        }
        return true;
    }

    uint16_t getData () const {
        return m_data;
    };
    void setData(const uint16_t & data) {
        m_data = data;
    };

    Tag_UINT16( const Constants::TagInfo & tag_info ) : 
        Tag(tag_info),
        m_data(0) {}

private:

    uint16_t m_data;
};

/**
 * Tag for uint16 types
 */
class Tag_UINT8 : public Tag {
public:
    virtual void setTag (ExifData *ed) const override {

    }

    virtual bool getTag (ExifData * ed) {
        ExifEntry *entry = exif_content_get_entry(ed->ifd[m_tag_info.ifd], static_cast<ExifTag>(m_tag_info.tag)); //points to exif data, do not delete.
        if (entry) {
            m_data = *(reinterpret_cast<uint8_t *> (entry->data));
        } else {
            return false;
        }
        return true;
    }

    uint8_t getData () const {
        return m_data;
    };
    void setData(const uint8_t & data) {
        m_data = data;
    };

    Tag_UINT8( const Constants::TagInfo & tag_info ) : Tag(tag_info) {}

private:

    uint8_t m_data;
};

/**
 * Tag for unsigned double types
 */
class Tag_UDOUBLE : public Tag {
public:
    virtual void setTag (ExifData *ed) const override {

    }

    virtual bool getTag (ExifData * ed) {
        ExifEntry *entry = exif_content_get_entry(ed->ifd[m_tag_info.ifd], static_cast<ExifTag>(m_tag_info.tag)); //points to exif data, do not delete.
        if (entry) {
            const ExifByteOrder o = exif_data_get_byte_order (entry->parent->parent);
            ExifRational rat = exif_get_rational (entry->data, o);
            m_data = static_cast<double>(rat.numerator)/static_cast<double>(rat.denominator);
        } else {
            return false;
        }
        return true;
    }

    double getData () const {
        return m_data;
    };
    void setData(const double & data) {
        m_data = std::abs(data);
    };

    Tag_UDOUBLE( const Constants::TagInfo & tag_info ) : 
        Tag(tag_info),
        m_data(0.0) {}

private:

    double m_data;
};

/**
 * Tag for signed double types
 */
class Tag_DOUBLE : public Tag {
public:
    virtual void setTag (ExifData *ed) const override {

    }

    virtual bool getTag (ExifData * ed) {
        ExifEntry *entry = exif_content_get_entry(ed->ifd[m_tag_info.ifd], static_cast<ExifTag>(m_tag_info.tag)); //points to exif data, do not delete.
        if (entry) {
            m_data = *(reinterpret_cast<double *> (entry->data));
        } else {
            return false;
        }
        return true;
    }

    double getData () const {
        return m_data;
    };
    void setData(const double & data) {
        m_data = data;
    };

    Tag_DOUBLE( const Constants::TagInfo & tag_info ) : 
        Tag(tag_info),
        m_data(0.0) {}

private:

    double m_data;
};

/**
 * Tag for string types
 */
class Tag_STRING : public Tag {
public:
    virtual void setTag (ExifData *ed) const override {

    }

    virtual bool getTag (ExifData * ed) {
        ExifEntry *entry = exif_content_get_entry(ed->ifd[m_tag_info.ifd], static_cast<ExifTag>(m_tag_info.tag)); //points to exif data, do not delete.
        if (entry) {
            if (entry->size > 0) {
                m_data = std::string (reinterpret_cast<char *> (entry->data), entry->size-1);
            } else {
                m_data = "";
            }
        } else {
            return false;
        }
        return true;
    }

    std::string getData () const {
        return m_data;
    };
    void setData(const std::string & data) {
        m_data = data;
    };

    Tag_STRING( const Constants::TagInfo & tag_info ) : 
        Tag(tag_info),
        m_data("") {}

private:

    std::string m_data;
};

/**
 * Tag for uint16 vector/array types
 */
class Tag_UINT16_ARRAY : public Tag {
public:
    virtual void setTag (ExifData *ed) const override {
        
    }

    virtual bool getTag (ExifData * ed) {
        ExifEntry *entry = exif_content_get_entry(ed->ifd[m_tag_info.ifd], static_cast<ExifTag>(m_tag_info.tag)); //points to exif data, do not delete.
        if (entry) {
            m_data = std::vector<uint16_t> (reinterpret_cast<uint16_t *> (entry->data), reinterpret_cast<uint16_t *> (entry->data) + entry->size/sizeof(uint16_t));
        } else {
            return false;
        }
        return true;
    }

    std::vector <uint16_t> getData () const {
        return m_data;
    };
    void setData(const std::vector <uint16_t> & data) {
        m_data = data;
    };

    Tag_UINT16_ARRAY( const Constants::TagInfo & tag_info ) : 
        Tag(tag_info),
        m_data() {}

private:

    std::vector <uint16_t> m_data;
};

/**
 * Tag for unsigned double array types
 */
class Tag_UDOUBLE_ARRAY : public Tag{
public:
    virtual void setTag (ExifData *exif) const override {

    }

    virtual bool getTag (ExifData * ed) {
        ExifEntry *entry = exif_content_get_entry(ed->ifd[m_tag_info.ifd], static_cast<ExifTag>(m_tag_info.tag)); //points to exif data, do not delete.
        if (entry) {
            int vec_size (entry->size/sizeof(ExifRational));
            m_data.clear();
            m_data.reserve(vec_size);
            for (int i = 0; i < vec_size; ++i) {
                ExifRational * rat = &(reinterpret_cast<ExifRational*> (entry->data)[i]);
                m_data.push_back(static_cast<double>(rat->numerator)/static_cast<double>(rat->denominator));
            }
        } else {
            return false;
        }
        return true;
    }
    std::vector <double> getData () const {
        return m_data;
    };
    void setData(const std::vector <double> & data) {
        m_data = data;
    };

    Tag_UDOUBLE_ARRAY( const Constants::TagInfo & tag_info ) : 
        Tag(tag_info),
        m_data() {}

private:

    std::vector <double> m_data;
};

/**
 * Tag for double array types (always signed)
 */
class Tag_DOUBLE_ARRAY : public Tag{
public:
    virtual void setTag (ExifData *exif) const override {

    }

    virtual bool getTag (ExifData * ed) {
        ExifEntry *entry = exif_content_get_entry(ed->ifd[m_tag_info.ifd], static_cast<ExifTag>(m_tag_info.tag)); //points to exif data, do not delete.
        if (entry) {
            m_data = std::vector<double> (reinterpret_cast<double *> (entry->data), reinterpret_cast<double *> (entry->data) + entry->size / sizeof(double));
        } else {
            return false;
        }
        return true;
    }
    std::vector <double> getData () const {
        return m_data;
    };
    void setData(const std::vector <double> & data) {
        m_data = data;
    };

    Tag_DOUBLE_ARRAY( const Constants::TagInfo & tag_info ) : 
        Tag(tag_info),
        m_data() {}

private:

    std::vector <double> m_data;
};


}
}