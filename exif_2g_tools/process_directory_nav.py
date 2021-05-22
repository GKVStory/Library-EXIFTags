from pathlib import Path
import numpy as np
import EXIFTagsPython as et
import datetime as dt
import PSONNAV_parser as psp

#depth_file = Path("G:\\Shared drives\\Engineering\\Projects\\2020.07 - Boat Testing\\2020-11-09 - PRO Laser PCO Dome Camera Boat Trip\\ProcessedNav\\Daytime-Images\\depth.csv")
#nav_file = Path("G:\\Shared drives\\Engineering\\Projects\\2020.07 - Boat Testing\\2020-11-09 - PRO Laser PCO Dome Camera Boat Trip\\ProcessedNav\\Daytime-Images\\psonnav20201122134507.asc")
input_directory = Path("G:\\Shared drives\\Engineering\\ImageDatasets\\2019-04-UlsterUniversity_Marine Habitats\\2019-04-30-Empire-Heritage-1\\Matt_Mono_LightLevel")
output_directory = Path("G:\\Shared drives\\Engineering\\ImageDatasets\\2019-04-UlsterUniversity_Marine Habitats\\2019-04-30-Empire-Heritage-1\\Matt_Mono_LightLevel-WithCalTags")

if (not output_directory.exists()):
    output_directory.mkdir(exist_ok=True, parents=True)

in_files = [a_file for a_file in input_directory.rglob("*_raw_*.tif")]

out_files = [output_directory / a_file.parts[-1] for a_file in in_files]

#depth = np.genfromtxt(str(depth_file), delimiter=",", invalid_raise=False)
#nav = psp.load_psonnav(str(nav_file))

error_message = ""
for i, (in_file, out_file) in enumerate (zip(in_files, out_files)):
    print (f"{i/len(in_files)} % --- {str(in_file)} --- {str(out_file)}")
    tags = et.Tags()
    if (not tags.load_header (str(in_file), error_message)):
        raise RuntimeError(error_message)
    
    image_time = tags.pps_time/1.0E6 

    #time_index = np.searchsorted(depth[:, 0], image_time)
    #nav_index = np.searchsorted(nav, image_time )

    tags.pps_time = int(image_time*1E6)
    tags.date_time = int(image_time*1E6)
    
    try:
        #tags.subject_distance = depth[time_index, 1]
        #lat = nav[nav_index].latitude
        #lon = nav[nav_index].longitude
        #altitude = nav[nav_index].depth
        #tags.latitude = np.abs(lat)
        #tags.longitude = np.abs(lon)
        #tags.altitude = np.abs(altitude)
        #tags.subject_distance = np.abs(depth[time_index, 1])
        #tags.vehicle_altitude = np.abs(depth[time_index, 1])
        #tags.latitude_ref = et.LATITUDEREF_NORTH if lat > 0 else et.LATITUDEREF_SOUTH
        #tags.longitude_ref = et.LONGITUDEREF_EAST if lon > 0 else et.LONGITUDEREF_WEST
        #tags.altitude_ref = et.ALTITUDEREF_BELOW_SEA_LEVEL
        tags.camera_matrix = [1589.722946047685, 1588.880439492318, 1057.544061898254, 1014.516740933401]
        tags.distortion = [-0.1016980584878131, 0.1366924578854253, -0.0002396759220364037, -0.0001685147503147413, -0.03428109583690397]
        tags.lens_model = "40-0444"
        tags.model = "40-0026"
        tags.bayer_pattern = et.BayerPatternType.BAYER_GRAYSCALE
        tags.viewport_type = et.ViewportType.VIEWPORT_FLAT
        tags.pixel_size = [6500, 6500]
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
        #tags.exposure_time = 2.0
        #tags.pose =[nav[nav_index].roll, nav[nav_index].pitch, nav[nav_index].heading]


        et.save_tags (tags, str(in_file), str(out_file))
    except Exception as e:
        print (e)
