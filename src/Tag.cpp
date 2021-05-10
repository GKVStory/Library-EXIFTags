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
        case Constants::UDOUBLE:
            return std::make_unique<Tag_UDOUBLE> (Constants::TAG_INFO[tag]);
        case Constants::DOUBLE:
            return std::make_unique<Tag_DOUBLE> (Constants::TAG_INFO[tag]);
        case Constants::STRING:
            return std::make_unique<Tag_STRING> (Constants::TAG_INFO[tag]);
		case Constants::UINT32_ARRAY:
            return std::make_unique<Tag_UINT32_ARRAY> (Constants::TAG_INFO[tag]);
        case Constants::UINT16_ARRAY:
            return std::make_unique<Tag_UINT16_ARRAY> (Constants::TAG_INFO[tag]);
		case Constants::UINT8_ARRAY:
            return std::make_unique<Tag_UINT8_ARRAY> (Constants::TAG_INFO[tag]);
        case Constants::DOUBLE_ARRAY:
            return std::make_unique<Tag_DOUBLE_ARRAY> (Constants::TAG_INFO[tag]);
        case Constants::UDOUBLE_ARRAY:
            return std::make_unique<Tag_UDOUBLE_ARRAY> (Constants::TAG_INFO[tag]);
        default:
            return nullptr;
    }
    
}


/* Get an existing tag, or create one if it doesn't exist */
ExifEntry *Tag::initTag(ExifData *exif, ExifIfd ifd, ExifTag tag) {
	ExifEntry *entry;
	/* Return an existing tag if one exists */
	if (!((entry = exif_content_get_entry (exif->ifd[ifd], tag)))) {
	    /* Allocate a new entry */
	    entry = exif_entry_new ();
	    if (!entry) {
            return nullptr;
        }
	    entry->tag = tag; /* tag must be set before calling
				 exif_content_add_entry */

	    /* Attach the ExifEntry to an IFD */
	    exif_content_add_entry (exif->ifd[ifd], entry);

	    /* Allocate memory for the entry and fill with default data */
	    exif_entry_initialize (entry, tag);

	    /* Ownership of the ExifEntry has now been passed to the IFD.
	     * One must be very careful in accessing a structure after
	     * unref'ing it; in this case, we know "entry" won't be freed
	     * because the reference count was bumped when it was added to
	     * the IFD.
	     */
	    exif_entry_unref(entry);
	}
	return entry;
}

/* Create a brand-new tag with a data field of the given length, in the
 * given IFD. This is needed when exif_entry_initialize() isn't able to create
 * this type of tag itself, or the default data length it creates isn't the
 * correct length.
 */
ExifEntry *Tag::createTag(ExifData *exif, ExifIfd ifd, ExifTag tag, size_t len) {
	void *buf;
	ExifEntry *entry;
	
	/* Create a memory allocator to manage this ExifEntry */
	ExifMem *mem = exif_mem_new_default();
	if (!mem) {
        return nullptr;
    }

	/* Create a new ExifEntry using our allocator */
	entry = exif_entry_new_mem (mem);
	if (!entry) {
        exif_mem_unref(mem);
        return nullptr;
    }

	/* Allocate memory to use for holding the tag data */
	buf = exif_mem_alloc(mem, (ExifLong)len);
	if (!buf) {
         exif_mem_unref(mem);
         exif_entry_unref(entry);
         return nullptr;
    }

	/* Fill in the entry */
	entry->data = reinterpret_cast<unsigned char *> (buf);
	entry->size = static_cast<unsigned int>(len);
	entry->tag = tag;
	entry->components = static_cast<unsigned long>(len);
	entry->format = EXIF_FORMAT_UNDEFINED;

	/* Attach the ExifEntry to an IFD */
	exif_content_add_entry (exif->ifd[ifd], entry);

	/* The ExifMem and ExifEntry are now owned elsewhere */
	exif_mem_unref(mem);
	exif_entry_unref(entry);

	return entry;
}

} //tags
} //tg
