cmake_minimum_required(VERSION 3.13.1)
project(ContainerFS)

# Load libzip
find_package(ZLIB)

# Load fuse3
find_path(FUSE3_INCLUDE_DIRS "fuse3/fuse.h")
set(FUSE3_INCLUDE_DIRS "${FUSE3_INCLUDE_DIRS}/fuse3")
find_library(FUSE3_LIBRARIES fuse3)
message("-- Found FUSE3: ${FUSE3_LIBRARIES}")

# Set include directories
file(GLOB FZIP_INCLUDE_DIRS "src/*.h")
include_directories(include ${FZIP_INCLUDE_DIRS} ${FUSE3_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS})

# Shared data library
add_library(data STATIC src/data.c)
target_link_libraries(data ${ZLIB_LIBRARIES})

# Fzip main library
add_library(fzip STATIC src/fzip.c src/readdir.c)
target_link_libraries(fzip data ${ZLIB_LIBRARIES} ${FUSE3_LIBRARIES})

# Utility library
add_library(utility STATIC src/utility.c)

# Executable
add_executable(zipfs src/main.c)
target_link_libraries(zipfs data fzip utility)