//!
//! # exif_2g
//! 
//! This library is used to parse and create the exif tags used in 2G Robotics Tiff and JPG files.
//! 
//! 


/// Utility function for  parsing the Exif tags in a file.
pub fn parse_tags (filename: &str) -> Result<exif::Exif, exif::Error> {
    let file = std::fs::File::open(filename)?;
    let mut bufreader = std::io::BufReader::new(&file);
    let exifreader = exif::Reader::new();
    exifreader.read_from_container(&mut bufreader)
}

pub fn 

#[cfg(test)]
mod tests {
    use super::*;

    
    const TEST_JPG: &str = concat!(env!("CARGO_MANIFEST_DIR"), "/../test_data/", "exif.jpg");
    const TEST_TIF: &str = concat!(env!("CARGO_MANIFEST_DIR"), "/../test_data/", "exif.tif");

    #[test]
    fn test_modify_tags() -> Result<(), exif::Error> {
        for path in &[TEST_JPG, TEST_TIF] {
            let file = std::fs::File::open(path)?;
            let mut bufreader = std::io::BufReader::new(&file);
            let exifreader = exif::Reader::new();
            let exif = exifreader.read_from_container(&mut bufreader)?;
            for f in exif.fields() {
                println!("{} {} {}",
                         f.tag, f.ifd_num, f.display_value().with_unit(&exif));
            }
        }
        Ok(())
    }
}
