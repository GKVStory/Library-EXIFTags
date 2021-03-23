from pathlib import Path
import numpy as np
import EXIFTagsLibPython as et
import datetime as dt

depth_file = Path("D:\\_Investigations\\2021-03-19-taggingImages\\depth.csv")
nav_file = Path("D:\\_Investigations\\2021-03-19-taggingImages\\navigation.csv")
input_directory = Path("D:\\_Investigations\\2021-03-19-taggingImages\\Colour-4ms-multipletargets-multipledepths hodgepodge")
output_directory = Path("D:\\_Investigations\\2021-03-19-taggingImages\\Colour-4ms-multipletargets-multipledepths-hodgepodge-withDepth")

datetime_offset = (dt.datetime(year=2020, month = 12, day = 10, hour = 1, minute = 40, second = 20) - dt.datetime.utcfromtimestamp(0)).total_seconds()
print (datetime_offset)

if (not output_directory.exists()):
    output_directory.mkdir(exist_ok=True, parents=True)

in_files = [a_file for a_file in input_directory.rglob("*.tif")]

out_files = [output_directory / a_file.parts[-1] for a_file in in_files]

depth = np.genfromtxt(str(depth_file), delimiter=",", invalid_raise=False)
nav = np.genfromtxt(str(nav_file), delimiter=",", invalid_raise=False)

error_message = ""
for i, (in_file, out_file) in enumerate (zip(in_files, out_files)):
    print (f"{i/len(in_files)} % --- {str(in_file)} --- {str(out_file)}")
    tags = et.Tags()
    if (not tags.load_header (str(in_file), error_message)):
        raise RuntimeError(error_message)
    
    image_time = tags.pps_time/1.0E6 + datetime_offset

    time_index = np.searchsorted(depth[:, 0], image_time)
    nav_index = np.searchsorted(nav[:, 0], image_time*1E6)

    tags.pps_time = int(image_time*1E6)
    tags.date_time = int(image_time*1E6)
    
    try:
        tags.subject_distance = depth[time_index, 1]
        lat = nav[nav_index, 1]
        lon = nav[nav_index, 2]
        altitude = nav[nav_index, 3]
        tags.latitude = np.abs(lat)
        tags.longitude = np.abs(lon)
        tags.altitude = np.abs(altitude)
        tags.latitudeRef = et.LATITUDEREF_NORTH if lat > 0 else et.LATITUDEREF_SOUTH
        tags.latitudeRef = et.LONGITUDEREF_EAST if lon > 0 else et.LONGITUDEREF_WEST
        tags.altitudeRef = et.ALTITUDEREF_ABOVE_SEA_LEVEL
        et.save_tags (tags, str(in_file), str(out_file))
    except Exception as e:
        print (e)
