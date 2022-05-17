from conans import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, CMakeDeps
from conan.tools.layout import basic_layout, cmake_layout
from conan.tools.build import cross_building
from conan.tools.files import copy

class EXIFTags(ConanFile):
    name = "EXIFTags"
    version = "6.0.0"
    license = "Voyis"
    url = "https://github.com/Voyis/Library-EXIFTags"
    description = "This library handles the reading and writing of the Voyis EXIF tags for both jpg and tiff images."

    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = {"shared": False}
    generators = "CMakeToolchain", "CMakeDeps"
    exports_sources = "include/*", "src/*", "tests/*", "lib/*", "test_data/*", "*.cmake", "CMakeLists.txt", "*.md"
        
    def requirements(self):        
        # todo: ENABLE THIS ONLY WHEN TESTING
        self.requires.add('gtest/[>=1.8.0]')
        self.requires.add('libtiff/4.3.0')
        # opencv as well... this lib is standalone
     
        # if linux, this will just use the system opencv. cuda NOT needed
        if self.settings.os == "Windows":
            self.requires.add('opencv-2g/4.5.1')

   
    def layout(self):
        cmake_layout(self)
    
    def imports(self):
        self.copy("*.dll", "", "bin")
        self.copy("*.dylib", "", "lib")

        # Conan 2.0 will do this:
        # for dep in self.dependencies.values():
        #     copy(self, "*.dylib", dep.cpp_info.libdirs[0], self.build_folder)
        #     copy(self, "*.dll", dep.cpp_info.libdirs[0], self.build_folder)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["BUILD_TESTS"] = True
        tc.generate()

        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
         self.copy("*.h*", dst="include", src="include")
         self.copy("*.h*", dst="include", src="lib/libexif")
         self.copy("*.lib", dst="lib", keep_path=False)
         self.copy("*.dll", dst="bin", keep_path=False)
         self.copy("*.dylib*", dst="lib", keep_path=False)
         self.copy("*.so", dst="lib", keep_path=False)
         self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["EXIFTags", "libexif"]