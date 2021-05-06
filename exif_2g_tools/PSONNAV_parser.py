import datetime as dt
from enum import IntEnum
from os import DirEntry
from pathlib import Path
from functools import total_ordering

def load_psonnav(filename_full, day=0, month=0, year=0):
    """
    This function return a list of PSONNAV entries, and uses the filename to extract the original datetime (don't change the filenames.)
    Assumes the following format: psonnav20201122134507.asc unless you provide a day, month and year arguments
    """
    filename = Path(filename_full)

    if day==0 and month == 0 and year == 0:
        year = int(filename.name[7:11])
        month = int(filename.name[11:13])
        day = int(filename.name[13:15])

    data = []
    with open (str(filename), "r" ) as fid:
        for line in fid.readlines():
            entry = PSONNAVEntry() 
            try:
                entry.parse_line(line, year=year, day=day, month=month)
                data.append(entry)
            except:
                next

    return data

class PSONNAVFields (IntEnum):
    TIME=1
    LATITUDE=2
    LATITUDE_REF=3
    LONGITUDE=4
    LONGITUDE_REF=5
    POSITION_ERROR_MAJOR=6
    POSITION_ERROR_MINOR=7
    POSITION_ERROR_DIRECTION=8
    POSITION_STATUS=8
    DEPTH=10
    DEPTH_STDDEV=11
    ROLL=12
    PITCH=13
    HEADING=14
    HEADING_STDDEV=15
    ORIENTATION_STATUS=16

@total_ordering
class PSONNAVEntry:
    def __init__ (self, time=0, lat=0.0, lon=0.0, depth=0.0, roll=0.0, pitch=0.0, heading=0.0):
        self._time = time
        self._lat = lat
        self._lon = lon
        self._depth = depth
        self._roll = roll
        self._pitch = pitch
        self._heading = heading

    def deg_min_to_dec_deg(self, deg_min_string):
        """
        Utility function to turn deg_min lat or long format into decimal degrees.
        """
        offset = 3
        if deg_min_string.index('.') == 4:
            #latitude
            offset=2
        return float(deg_min_string[0:offset]) + float(deg_min_string[offset:]) / 60.0

    def parse_line (self, line_string, day, month, year):
        """
        Given a string, parse it as if it was a PSONNAV entry. The day/month/year are what the file time is referenced to.
        Raise RuntimeError if the line is not a PSONNAV entry.
        """
        if "$PSONNAV" not in line_string:
            raise RuntimeError("Line is not a PSONNAV entry.")
        day_time = dt.datetime(year=year, month=month, day=day)
        elements = line_string.split(",")
        self._time = float(elements[PSONNAVFields.TIME]) + day_time.timestamp()
        lat_string = elements[PSONNAVFields.LATITUDE]
        lon_string = elements[PSONNAVFields.LONGITUDE]
        self._lat = self.deg_min_to_dec_deg (lat_string) * (1.0 if elements[PSONNAVFields.LATITUDE_REF] == "N" else -1)
        self._lon = self.deg_min_to_dec_deg (lon_string) * (1.0 if elements[PSONNAVFields.LONGITUDE_REF] == "E" else -1)
        self._depth = float(elements[PSONNAVFields.DEPTH])
        self._roll = float(elements[PSONNAVFields.ROLL])
        self._pitch = float(elements[PSONNAVFields.PITCH])
        self._heading = float(elements[PSONNAVFields.HEADING])

    def __eq__(self, other):
        if isinstance(other, PSONNAVEntry):
            return self._time == other._time
        return False
    
    def __lt__(self, other):
        if isinstance(other, PSONNAVEntry):
            return self._time < other._time
        return False

    @property
    def time(self):
        return self._time

    @property
    def latitude(self):
        return self._lat

    @property
    def longitude(self):
        return self._lon

    @property 
    def depth(self):
        return self._depth

    @property 
    def altitude(self):
        return -self._depth

    @property
    def roll(self):   
        return self._roll

    @property
    def pitch(self):
        return self._pitch

    @property
    def heading(self):
        return self._heading

def test_parse():
    from pytest import approx
    line = "$PSONNAV,134507.304,4444.918260,S,08108.301280,W,0.889,0.729,209.57,A,0.828,0.049,-0.437,-0.442,187.912,0.066,A,IDV,,,,,*22" 

    entry = PSONNAVEntry()
    entry.parse_line(line, day=22, month=11, year=2020)

    assert (entry.time == dt.datetime(year=2020, month=11, day = 23, hour = 13, minute = 21, second = 47, microsecond=304000).timestamp())
    approx (entry.latitude, -44.748637666)
    approx (entry.longitude, 81.1383546666)
    approx (entry.depth, 0.828)
    approx (entry.roll, -0.437)
    approx (entry.pitch, -0.442)
    approx (entry.heading, 187.912)
