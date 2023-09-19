from conan import ConanFile
from conan.tools.files import copy
from conan.tools.cmake import cmake_layout, CMake
from conan.tools.build import check_min_cppstd


class Callables(ConanFile):
    name = "callables"
    version = "0.2.0"
    # package_type = "library"

    # Optional metadata
    license = "BSL-1.0"
    author = "Brugo Riccardo brugo.riccardo@gmail.com"
    url = "https://github.com/rbrugo/callables"
    description = "A set of composable and partially-applicable function objects " \
        "to simplify the use of algorithms and views in C++20 and later "
    topics = ("c++", "c++20", "c++23", "cpp", "ranges")

    # Binary configuration: not necessary, this is header only
    # settings = "os", "arch", "compiler", "build_type"

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "include/*"
    no_copy_source = True
    # generators = "CMakeToolchain", "CMakeDeps"

    # def validate(self):
    #     check_min_cppstd(self, 20)

    # def layout(self):
    #     cmake_layout(self)

    # def build(self):
    #     if not self.conf.get("tools.build:skip_test", default=True):
    #         cmake = CMake(self)
    #         # cmake.configure(build_script_folder="test")
    #         cmake.configure()
    #         cmake.build()
    #         # self.run(os.path.join(self.cpp.build.bindir, "test_sum"))
    #         cmake.test()

    def package(self):
        # This will also copy the "include" folder
        copy(self, "*.hpp", self.source_folder, self.package_folder)

    def package_info(self):
        # For header-only packages, libdirs and bindirs are not used
        # so it's necessary to set those as empty.
        self.cpp_info.bindirs = []
        self.cpp_info.libdirs = []
    #
    # def package_id(self):
    #     self.info.clear()
