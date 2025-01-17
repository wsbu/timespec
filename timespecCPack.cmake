# (C)2019 Red Lion Controls, Inc. All rights reserved. Red Lion, the Red Lion logo and Sixnet are registered trademarks
# of Red Lion Controls, Inc. All other company and product names are trademarks of their respective owners.

set(CPACK_GENERATOR
    TGZ
    ZIP
)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY 0)
set(CPACK_INSTALL_CMAKE_PROJECTS
    "${CMAKE_BINARY_DIR}"
    "${PROJECT_NAME}"
    ALL
    .
)

set(CPACK_PROJECT_URL "https://bitbucket.org/redlionstl/timespec")
set(CPACK_PACKAGE_VENDOR "Red Lion Controls")
set(CPACK_PACKAGE_CONTACT "Lance Jennings <lance.jennings@redlion.net>")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION
    ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
if (PROJECT_VERSION_TWEAK)
    set(CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION}.${PROJECT_VERSION_TWEAK})
endif ()
set(CPACK_PACKAGE_RELOCATABLE ON)

include(CPack)
