use clap::{Arg, App};

fn main() {
    let matches = App::new("parse_tags")
        .version("0.0.1")
        .author("Ryan Wicks <ryancwicks@gmail.com>")
        .about("Parses exif tags in jpg and tif images.")
        .arg(Arg::with_name("INPUT")
                .help("Sets the input file to use")
                .required(true)
                .index(1))
        .get_matches();

    let filename = matches.value_of("INPUT").unwrap();

    let exif_tags = exif_2g::parse_tags (&filename).unwrap_or_else (|err| {
        println!("Could not load exif tags from {0}: \n {1}", filename, err);
        std::process::exit(-1);
    }) ;

    for f in exif_tags.fields() {
        println!("{} {} {}",
            f.tag, f.ifd_num, f.display_value().with_unit(&exif_tags));
    }
}