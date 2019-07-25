# (C)2019 Red Lion Controls, Inc. All rights reserved. Red Lion, the Red Lion logo and Sixnet are registered trademarks
# of Red Lion Controls, Inc. All other company and product names are trademarks of their respective owners.

from conans import ConanFile, CMake, tools


class timespec(ConanFile):
    name = 'timespec'
    version = tools.load("version.txt").strip()
    license = 'Proprietary'
    url = 'https://bitbucket.org/redlionstl/timespec'
    description = 'Functions for working with timespec structures'
    settings = 'os', 'compiler', 'build_type', 'arch'
    options = {
        'shared': [True, False],
    }
    default_options = 'shared=True'
    generators = 'cmake'

    exports = 'version.txt'
    scm = {
        'type': 'git',
        'url': 'auto',
        'revision': 'auto'
    }

    def build(self):
        cmake = self.cmake
        cmake.configure()
        cmake.build()
        cmake.test()

    def package(self):
        self.cmake.install()

    def package_info(self):
        self.cpp_info.libs = ['timespec']

    @property
    def cmake(self):
        return CMake(self)
