cmake_minimum_required(VERSION 3.6)
project(ContainerFS)
set (CMAKE_C_STANDARD 11)

# Load libzip
find_path(LIBZIP_INCLUDE_DIRS "zip/zip.h")
set(LIBZIP_INCLUDE_DIRS "${LIBZIP_INCLUDE_DIRS}/zip")
find_library(LIBZIP_LIBRARIES zip)
message("-- Found LIBZIP: ${LIBZIP_LIBRARIES}")

# Load fuse3
find_path(FUSE3_INCLUDE_DIRS "fuse3/fuse.h")
set(FUSE3_INCLUDE_DIRS "${FUSE3_INCLUDE_DIRS}/fuse3")
find_library(FUSE3_LIBRARIES fuse3)
message("-- Found FUSE3: ${FUSE3_LIBRARIES}")

# Set include directories
set(FZIP_INCLUDE_DIRS "src/")
include_directories(include ${FZIP_INCLUDE_DIRS} ${FUSE3_INCLUDE_DIRS} ${LIBZIP_INCLUDE_DIRS})

# DirTree library
add_library(dir_tree STATIC src/dir_tree.c)
target_link_libraries(dir_tree ${LIBZIP_LIBRARIES})

# Shared data library
add_library(data STATIC src/data.c)
target_link_libraries(data dir_tree pthread)

# Utility library
add_library(utility STATIC src/utility.c)

# Fzip main library
add_library(fzip STATIC src/fzip.c
                        src/init.c
                        src/readdir.c
                        src/getattr.c
                        src/read.c
                        src/open.c
                        src/readlink.c
                        src/release.c)
target_link_libraries(fzip ${FUSE3_LIBRARIES} data pthread)

# Executable
add_executable(zipfs src/main.c)
target_link_libraries(zipfs data fzip utility pthread)
