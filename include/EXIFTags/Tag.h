#pragma once
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

extern "C" {
#include <libexif/exif-data.h>
#include <libexif/exif-entry.h>
}
#include <vector>
#include <string>
#include <memory>
#include <cmath>
#include <cstring>

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

    /**
     * Is the parameter set (either loaded from a file or set directly)
     * @return bool is the parameter set.
     */
    bool isSet() const {
        return m_is_set;
    };

protected:
    //Tag constructor //stores a reference to the tag structure. This is a reference, 
    //and should only reference something with the same lifetime as the program, like the 
    //TAG_INFO vector in TagConstants.h
    Tag( const Constants::TagInfo & tag_info ) : 
        m_tag_info(tag_info),
        m_is_set(false)
        {};

    Constants::TagInfo m_tag_info;
    bool m_is_set;

    /**
     * Get an existing tag, or create one if it doesn't exist
     * @param [in/out] pointer to exif data structure 
     * @param ifd to store the entry in.
     * @param exiftag to be created/loaded
     * @return pointer to the exif entry, memory is managed by the exif data if not null
     */
    static ExifEntry *initTag(ExifData *exif, ExifIfd ifd, ExifTag tag);

    /**
     * Create a brand-new tag with a data field of the given length, in the
     * given IFD. This is needed when exif_entry_initialize() isn't able to create
     * this type of tag itself, or the default data length it creates isn't the
     * correct length.
     * @param [in/out] exif data structure 
     * @param [in] ifd ifd to place the entry in
     * @param [in] tag tag id
     * @param [in] size_t size of data, in bytes.
     * @return pointer to exif entry. If not null, the memory is managed by the exif structure.
     */
    static ExifEntry *createTag(ExifData *exif, ExifIfd ifd, ExifTag tag, size_t len);

};

/**
 * Tag for uint32 types
 */
class Tag_UINT32 : public Tag {
public:
    virtual void setTag (ExifData *exif) const override {
        if (m_is_set) {
            ExifEntry *entry = initTag (exif, m_tag_info.ifd, static_cast<ExifTag>(m_tag_info.tag));
            if (!entry) {
                return;
            }

            if (entry->format == EXIF_FORMAT_UNDEFINED ) {
                entry->components = 1;
		        entry->format = EXIF_FORMAT_LONG;
		        entry->size = exif_format_get_size (entry->format) * entry->components;
		        entry->data = reinterpret_cast <unsigned char *> (exif_entry_alloc (entry, entry->size));
		        if (!entry->data) { 
                    clear_entry(entry); 
                    return; 
                }
            }

            exif_set_long (entry->data, Constants::DEFAULT_BYTE_ORDER, m_data);
        }
    }

    virtual bool getTag (ExifData * ed) override {
        ExifEntry *entry = exif_content_get_entry(ed->ifd[m_tag_info.ifd], static_cast<ExifTag>(m_tag_info.tag)); //points to exif data, do not delete.
        if (entry) {
            const ExifByteOrder o = exif_data_get_byte_order (entry->parent->parent);
            switch (entry->size) {
                case 4: {
                    m_data = static_cast<uint32_t>(exif_get_long (entry->data, o));
                    m_is_set = true;
                    break;
                }
                case 2: {
                    m_data = static_cast<uint32_t>(exif_get_short (entry->data, o));
                    m_is_set = true;
                    break;
                }
                case 1: {
                    m_data = static_cast<uint32_t>(*(reinterpret_cast<uint8_t *> (entry->data)));
                    m_is_set = true;
                    break;
                }
                default: {
                    break; //hopefully, we never get here.
                }
            }
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
        m_is_set = true;
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
        if (m_is_set) {
            ExifEntry *entry = initTag (exif, m_tag_info.ifd, static_cast<ExifTag>(m_tag_info.tag));
            if (!entry) {
                return;
            }
            if (entry->format == EXIF_FORMAT_UNDEFINED) {
                entry->components = 1;
		        entry->format = EXIF_FORMAT_SHORT;
		        entry->size = exif_format_get_size (entry->format) * entry->components;
		        entry->data = reinterpret_cast<unsigned char *> (exif_entry_alloc (entry, entry->size));
		        if (!entry->data) { 
                    clear_entry(entry); 
                    return;
                }
            }
            exif_set_short (entry->data, Constants::DEFAULT_BYTE_ORDER, m_data);
        }
    }

    virtual bool getTag (ExifData * ed) {
        ExifEntry *entry = exif_content_get_entry(ed->ifd[m_tag_info.ifd], static_cast<ExifTag>(m_tag_info.tag)); //points to exif data, do not delete.
        if (entry) {
            const ExifByteOrder o = exif_data_get_byte_order (entry->parent->parent);
            switch (entry->size) {
                case 4: {
                    m_data = static_cast<uint16_t>(exif_get_long (entry->data, o));
                    m_is_set = true;
                    break;
                }
                case 2: {
                    m_data = static_cast<uint16_t>(exif_get_short (entry->data, o));
                    m_is_set = true;
                    break;
                }
                case 1: {
                    m_data = static_cast<uint16_t>(*(reinterpret_cast<uint8_t *> (entry->data)));
                    m_is_set = true;
                    break;
                }
                default: {
                    break; //hopefully, we never get here.
                }
            }
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
        m_is_set = true;
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
    virtual void setTag (ExifData *exif) const override {
        if (m_is_set) {
            ExifEntry *entry = createTag (exif, m_tag_info.ifd, static_cast<ExifTag>(m_tag_info.tag), sizeof(m_data));
            if (!entry) {
                return;
            }
            *(entry->data) = m_data;
        }
    }

    virtual bool getTag (ExifData * ed) {
        ExifEntry *entry = exif_content_get_entry(ed->ifd[m_tag_info.ifd], static_cast<ExifTag>(m_tag_info.tag)); //points to exif data, do not delete.
        if (entry) {
            switch (entry->size) {
                case 4: {
                    m_data = static_cast<uint8_t>(*(reinterpret_cast<uint32_t *> (entry->data)));
                    m_is_set = true;
                    break;
                }
                case 2: {
                    m_data = static_cast<uint8_t>(*(reinterpret_cast<uint16_t *> (entry->data)));
                    m_is_set = true;
                    break;
                }
                case 1: {
                    m_data = static_cast<uint8_t>(*(reinterpret_cast<uint8_t *> (entry->data)));
                    m_is_set = true;
                    break;
                }
                default: {
                    break; //hopefully, we never get here.
                }
            }
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
        m_is_set = true;
    };

    Tag_UINT8( const Constants::TagInfo & tag_info ) : 
        Tag(tag_info),
        m_data (0){}

private:

    uint8_t m_data;
};

/**
 * Tag for unsigned double types
 */
class Tag_UDOUBLE : public Tag {
public:
    virtual void setTag (ExifData *exif) const override {
        if (m_is_set) {
            ExifEntry *entry = initTag (exif, m_tag_info.ifd, static_cast<ExifTag>(m_tag_info.tag));
            if (!entry) {
                return;
            }
            if (entry->format == EXIF_FORMAT_UNDEFINED) {
                entry->components = 1;
		        entry->format = EXIF_FORMAT_RATIONAL;
		        entry->size = exif_format_get_size (entry->format) * entry->components;
		        entry->data = reinterpret_cast <unsigned char *> (exif_entry_alloc (entry, entry->size));
		        if (!entry->data) { 
                    clear_entry(entry); 
                    return;
                }
            }
            ExifRational val;
            val.numerator = static_cast<uint32_t> (m_data*1E6);
            val.denominator = 1000000;
            exif_set_rational (entry->data, Constants::DEFAULT_BYTE_ORDER, val);
        }
    }

    virtual bool getTag (ExifData * ed) {
        ExifEntry *entry = exif_content_get_entry(ed->ifd[m_tag_info.ifd], static_cast<ExifTag>(m_tag_info.tag)); //points to exif data, do not delete.
        if (entry) {
            const ExifByteOrder o = exif_data_get_byte_order (entry->parent->parent);
            ExifRational rat = exif_get_rational (entry->data, o);
            m_data = static_cast<double>(rat.numerator)/static_cast<double>(rat.denominator);
            m_is_set = true;
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
        m_is_set = true;
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
    virtual void setTag (ExifData *exif) const override {
        if (m_is_set) {
            ExifEntry *entry = createTag (exif, m_tag_info.ifd, static_cast<ExifTag>(m_tag_info.tag), sizeof (m_data));
            if (!entry) {
                return;
            }
            memcpy (entry->data, &m_data, sizeof (m_data));
        }
    }

    virtual bool getTag (ExifData * ed) {
        ExifEntry *entry = exif_content_get_entry(ed->ifd[m_tag_info.ifd], static_cast<ExifTag>(m_tag_info.tag)); //points to exif data, do not delete.
        if (entry) {
            m_data = *(reinterpret_cast<double *> (entry->data));
            m_is_set = true;
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
        m_is_set = true;
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
    virtual void setTag (ExifData *exif) const override {
        if (m_is_set) {
            ExifEntry *entry = createTag (exif, m_tag_info.ifd, static_cast<ExifTag>(m_tag_info.tag), sizeof (char) * (m_data.size()+1));
            if (!entry) {
                return;
            }
            memset(entry->data, 0, sizeof(char) * (m_data.size()+1));
            memcpy (entry->data, m_data.c_str(), sizeof (char) * m_data.size());
        }
    }

    virtual bool getTag (ExifData * ed) {
        ExifEntry *entry = exif_content_get_entry(ed->ifd[m_tag_info.ifd], static_cast<ExifTag>(m_tag_info.tag)); //points to exif data, do not delete.
        if (entry) {
            if (entry->size > 0) {
                m_data = std::string (reinterpret_cast<char *> (entry->data), entry->size-1);
                m_is_set = true;
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
        m_is_set = true;
    };

    Tag_STRING( const Constants::TagInfo & tag_info ) : 
        Tag(tag_info),
        m_data("") {}

private:

    std::string m_data;
};

/**
 * Tag for uint8 vector/array types
 */
class Tag_UINT8_ARRAY : public Tag {
public:
    virtual void setTag (ExifData *exif) const override {
        if (m_is_set) {
            ExifEntry *entry = createTag (exif, m_tag_info.ifd, static_cast<ExifTag>(m_tag_info.tag), sizeof (uint8_t) * m_data.size());
            if (!entry) {
                return;
            }
            memcpy (entry->data, m_data.data(), sizeof (uint8_t) * m_data.size());
        }
    }

    virtual bool getTag (ExifData * ed) {
        ExifEntry *entry = exif_content_get_entry(ed->ifd[m_tag_info.ifd], static_cast<ExifTag>(m_tag_info.tag)); //points to exif data, do not delete.
        if (entry) {
            m_data = std::vector<uint8_t> (reinterpret_cast<uint8_t *> (entry->data), reinterpret_cast<uint8_t *> (entry->data) + entry->size/sizeof(uint8_t));
            m_is_set = true;
        } else {
            return false;
        }
        return true;
    }

    std::vector <uint8_t> getData () const {
        return m_data;
    };
    void setData(const std::vector <uint8_t> & data) {
        m_data = data;
        m_is_set = true;
    };

    Tag_UINT8_ARRAY( const Constants::TagInfo & tag_info ) : 
        Tag(tag_info),
        m_data() {}

private:

    std::vector <uint8_t> m_data;
};


/**
 * Tag for uint16 vector/array types
 */
class Tag_UINT16_ARRAY : public Tag {
public:
    virtual void setTag (ExifData *exif) const override {
        if (m_is_set) {
            ExifEntry *entry = createTag (exif, m_tag_info.ifd, static_cast<ExifTag>(m_tag_info.tag), sizeof (uint16_t) * m_data.size());
            if (!entry) {
                return;
            }
            memcpy (entry->data, m_data.data(), sizeof (uint16_t) * m_data.size());
        }
    }

    virtual bool getTag (ExifData * ed) {
        ExifEntry *entry = exif_content_get_entry(ed->ifd[m_tag_info.ifd], static_cast<ExifTag>(m_tag_info.tag)); //points to exif data, do not delete.
        if (entry) {
            m_data = std::vector<uint16_t> (reinterpret_cast<uint16_t *> (entry->data), reinterpret_cast<uint16_t *> (entry->data) + entry->size/sizeof(uint16_t));
            m_is_set = true;
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
        m_is_set = true;
    };

    Tag_UINT16_ARRAY( const Constants::TagInfo & tag_info ) : 
        Tag(tag_info),
        m_data() {}

private:

    std::vector <uint16_t> m_data;
};

/**
 * Tag for uint32 vector/array types
 */
class Tag_UINT32_ARRAY : public Tag {
public:
    virtual void setTag (ExifData *exif) const override {
        if (m_is_set) {
            ExifEntry *entry = createTag (exif, m_tag_info.ifd, static_cast<ExifTag>(m_tag_info.tag), sizeof (uint32_t) * m_data.size());
            if (!entry) {
                return;
            }
            memcpy (entry->data, m_data.data(), sizeof (uint32_t) * m_data.size());
        }
    }

    virtual bool getTag (ExifData * ed) {
        ExifEntry *entry = exif_content_get_entry(ed->ifd[m_tag_info.ifd], static_cast<ExifTag>(m_tag_info.tag)); //points to exif data, do not delete.
        if (entry) {
            m_data = std::vector<uint32_t> (reinterpret_cast<uint32_t *> (entry->data), reinterpret_cast<uint32_t *> (entry->data) + entry->size/sizeof(uint32_t));
            m_is_set = true;
        } else {
            return false;
        }
        return true;
    }

    std::vector <uint32_t> getData () const {
        return m_data;
    };
    void setData(const std::vector <uint32_t> & data) {
        m_data = data;
        m_is_set = true;
    };

    Tag_UINT32_ARRAY( const Constants::TagInfo & tag_info ) : 
        Tag(tag_info),
        m_data() {}

private:

    std::vector <uint32_t> m_data;
};


/**
 * Tag for unsigned double array types
 */
class Tag_UDOUBLE_ARRAY : public Tag{
public:
    virtual void setTag (ExifData *exif) const override {
        if (m_is_set) {
            std::vector <ExifRational> temp_vec;
            temp_vec.reserve (m_data.size());

            for (const auto & elem : m_data) {
                ExifRational val;
                val.numerator = static_cast <uint32_t> (elem * 1E6);
                val.denominator = 1000000;
                temp_vec.push_back (val);
            }

            ExifEntry *entry = createTag (exif, m_tag_info.ifd, static_cast<ExifTag>(m_tag_info.tag), sizeof (ExifRational) * temp_vec.size());
            if (!entry) {
                return;
            }
            memcpy (entry->data, temp_vec.data(), sizeof (ExifRational) * temp_vec.size());
        }
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
            m_is_set = true;
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
        m_is_set = true;
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
        if (m_is_set) {
            ExifEntry *entry = createTag (exif, m_tag_info.ifd, static_cast<ExifTag>(m_tag_info.tag), sizeof (double) * m_data.size());
            if (!entry) {
                return;
            }
            memcpy (entry->data, m_data.data(), sizeof (double) * m_data.size());
        }
    }

    virtual bool getTag (ExifData * ed) {
        ExifEntry *entry = exif_content_get_entry(ed->ifd[m_tag_info.ifd], static_cast<ExifTag>(m_tag_info.tag)); //points to exif data, do not delete.
        if (entry) {
            m_data = std::vector<double> (reinterpret_cast<double *> (entry->data), reinterpret_cast<double *> (entry->data) + entry->size / sizeof(double));
            m_is_set = true;
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
        m_is_set = true;
    };

    Tag_DOUBLE_ARRAY( const Constants::TagInfo & tag_info ) : 
        Tag(tag_info),
        m_data() {}

private:

    std::vector <double> m_data;
};


}
}