# (C)2019 Red Lion Controls, Inc. All rights reserved. Red Lion, the Red Lion logo and Sixnet are registered trademarks
# of Red Lion Controls, Inc. All other company and product names are trademarks of their respective owners.

add_library(timespec timespec.c)
target_include_directories(timespec PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    $<INSTALL_INTERFACE:include>
)

install(TARGETS timespec EXPORT ${PROJECT_NAME}Config DESTINATION lib)

