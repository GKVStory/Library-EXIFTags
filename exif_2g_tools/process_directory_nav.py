from pathlib import Path
import numpy as np
import EXIFTagsPython as et
import datetime as dt
import PSONNAV_parser as psp
import cv2
import tempfile
import datetime
import re

depth_file = Path("D:\\_Investigations\\2021-06-04-MarjaritaData\\depth.csv")
nav_file = Path("D:\\_Investigations\\2021-06-04-MarjaritaData\\nav.csv")
input_directory = Path("D:\\_Investigations\\2021-06-04-MarjaritaData\\notsplit-processed")
output_directory = Path("D:\\_Investigations\\2021-06-04-MarjaritaData\\TaggedProcessed")

if (not output_directory.exists()):
    output_directory.mkdir(exist_ok=True, parents=True)

in_files = [a_file for a_file in input_directory.rglob("*_raw_*.jpg")]

out_files = [output_directory / Path(a_file.stem + "_raw_" + a_file.suffix ) for a_file in in_files]

depth = np.genfromtxt(str(depth_file), delimiter=",", invalid_raise=False)
nav = psp.load_psonnav(str(nav_file))
offset_time = (datetime.datetime( year=2020, month = 11, day = 10, hour = 1, minute = 40, second = 20 ) - datetime.datetime(year = 1970, month = 1, day = 1, hour = 0, minute = 0, second = 0)).total_seconds()
print (f"Offset time is {offset_time}")

def get_image_time_from_file(filename):
    """
    Get the image time from the filename in seconds
    """
    end_of_name = filename.split("T")[-1]
    num_part = float(end_of_name.split("_")[0])
    hour = np.floor(num_part / 10000)
    minute = np.floor((num_part - hour*10000) / 100 )
    second = num_part - hour * 10000 - minute * 100
    return hour * 3600 + minute * 60 + second
    

error_message = ""
#temp_file = tempfile.NamedTemporaryFile(delete=False, suffix=".tif")
for i, (in_file, out_file) in enumerate (zip(in_files, out_files)):
    print (f"{i/len(in_files)} % --- {str(in_file)} --- {str(out_file)}")
    tags = et.Tags()
    if (not tags.load_header (str(in_file), error_message)):
        next
    
    image_time = get_image_time_from_file(str(in_file)) + offset_time
    print (image_time)

    time_index = np.searchsorted(depth[:, 0], image_time)
    nav_index = np.searchsorted(nav[:, 0], image_time )

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
        tags.subject_distance = np.abs(depth[time_index, 1])
        tags.vehicle_altitude = np.abs(depth[time_index, 1])
        tags.latitude_ref = et.LATITUDEREF_NORTH if lat > 0 else et.LATITUDEREF_SOUTH
        tags.longitude_ref = et.LONGITUDEREF_EAST if lon > 0 else et.LONGITUDEREF_WEST
        tags.altitude_ref = et.ALTITUDEREF_BELOW_SEA_LEVEL
        tags.camera_matrix = [2095.500198060818, 2086.245746933741, 2070.726656348959, 1409.529270409561]
        tags.distortion = [-0.1647178859919031, 0.05694338809227908, -0.002486028296538326, -0.00238886046486664, -0.01055017371275293]
        tags.lens_model = "40-0444"
        tags.model = "40-3930"
        tags.serial_number = "40-3930"
        tags.bayer_pattern = et.BayerPatternType.BAYER_BG2BGR
        tags.viewport_type = et.ViewportType.VIEWPORT_DOMED
        tags.pixel_size = [3450, 3450]
        tags.focal_length=12.0
        tags.f_number=1.0/(2.0/12.0) #The f-number is the reciprocal of the relative aperture (the aperture diameter divided by focal length)
        tags.index_of_refraction = 1.34
        tags.frame_rate = 2.0
        tags.viewport_distance = -1
        tags.viewport_index = 1.472
        tags.viewport_thickness = -1
        tags.flash_energy = 0
        tags.flash = et.FlashType.FLASH_DIDNOTFIRE
        tags.light_source = et.LightSourceType.LIGHTSOURCE_DAYLIGHT
        tags.exposure_time = 2.0
        tags.pose =[nav[nav_index, 4], nav[nav_index, 5], nav[nav_index, 6]]
        tags.dvl = [depth[time_index, 2], depth[time_index, 3], depth[time_index, 4], depth[time_index, ],]


        #image = cv2.imread(str(in_file), cv2.IMREAD_GRAYSCALE + cv2.IMREAD_ANYDEPTH)
        #cv2.imwrite(temp_file.name, image, (cv2.IMWRITE_TIFF_COMPRESSION, 1))
        
        et.save_tags (tags, str(in_file), str(out_file))
    except Exception as e:
        print (e)
