from pathlib import Path
import numpy as np
import EXIFTagsLibPython as et
import datetime as dt

depth_file = Path("D:\\_Investigations\\2021-03-19-taggingImages\\depth.csv")
input_directory = Path("D:\\_Investigations\\2021-03-19-taggingImages\\Colour-4ms-multipletargets-multipledepths hodgepodge")
output_directory = Path("D:\\_Investigations\\2021-03-19-taggingImages\\Colour-4ms-multipletargets-multipledepths-hodgepodge-withDepth")

datetime_offset = (dt.datetime(year=2020, month = 12, day = 10, hour = 1, minute = 40, second = 20) - dt.datetime.utcfromtimestamp(0)).total_seconds()
print (datetime_offset)

if (not output_directory.exists()):
    output_directory.mkdir(exist_ok=True, parents=True)

in_files = [a_file for a_file in input_directory.rglob("*.tif")]

out_files = [output_directory / a_file.parts[-1] for a_file in in_files]

depth = np.genfromtxt(str(depth_file), delimiter=",", invalid_raise=False)

error_message = ""
for i, (in_file, out_file) in enumerate (zip(in_files, out_files)):
    print (f"{i/len(in_files)} % --- {str(in_file)} --- {str(out_file)}")
    tags = et.Tags()
    if (not tags.load_header (str(in_file), error_message)):
        raise RuntimeError(error_message)
    
    image_time = tags.pps_time/1.0E6 + datetime_offset

    time_index = np.searchsorted(depth[:, 0], image_time)

    tags.pps_time = int(image_time*1E6)
    tags.date_time = int(image_time*1E6)
    
    try:
        tags.subject_distance = depth[time_index, 1]
        et.save_tags (tags, str(in_file), str(out_file))
    except Exception as e:
        print (e)
