# Library-EXIFTags
This library handles the reading and writing of the 2G EXIF tags for both jpg and tiff images.

# Building

In addition to the core library, you can also build a command line tool that does some simple parsing to get time, GPS and altitude data from tagged images. You can also opt to build the test code and a set of python bindings. The python binding require a 64 bit build.

```
mkdir build
cmake .. -G "Visual Studio 14 Win64" -DBUILD_MAIN=<ON/OFF> -DBUILD_TESTS=<ON/OFF> -DBUILD_PYTHON=<ON/OFF>
cd build
cmake --build . --config=Release
```


