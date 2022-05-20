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

    options = {"shared": [True, False],
               "with_tests": [True, False],
               "with_cuda": [True, False]} # only applies to tests

    default_options = {"shared": False,
                       "with_tests": True,
                       "with_cuda": True}

    generators = "CMakeToolchain", "CMakeDeps"
    exports_sources = "include/*", "src/*", "tests/*", "lib/*", "test_data/*", "*.cmake", "CMakeLists.txt", "*.md"
    
    use_conan_opencv = True

    def requirements(self): 
        if self.options.with_tests:
            self.requires.add('gtest/[>=1.8.0]')
            self.requires.add('libtiff/4.3.0')

            if self.use_conan_opencv:

                # need to specify versions explicitly to resolve conflicts with libtiff
                self.requires.add('zlib/1.2.12')
                self.requires.add('libwebp/1.2.2')
                self.requires.add('libjpeg-turbo/2.1.2')
                self.requires.add('opencv/4.5.5')

                self.options["opencv"].contrib = True
                self.options["opencv"].with_jpeg = "libjpeg-turbo"
                self.options["libtiff"].jpeg = "libjpeg-turbo"

                if self.options.with_cuda:
                    self.options["opencv"].with_cuda = True
                    self.options["opencv"].cuda_arch_bin = self.env["CUDA_ARCH_BIN"]
                else:
                    self.options["opencv"].with_cuda = False
            

    def configure(self):
        if "BOARD" in self.env and self.env["BOARD"] == "Jetson-Xaiver-nx":
            self.use_conan_opencv = False

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
        tc.variables["BUILD_TESTS"] = self.options.with_tests
        tc.variables["USE_CONAN_OPENCV"] = self.use_conan_opencv
        tc.variables["WITH_CUDA"] =self.options.with_cuda
            
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