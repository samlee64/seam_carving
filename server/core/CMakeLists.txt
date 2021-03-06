cmake_minimum_required(VERSION 3.1)
project(a6)

add_subdirectory(src/ext)

# include directories
include_directories(${PROJECT_SOURCE_DIR}/src
                    ${PROJECT_SOURCE_DIR}/src/ext
                    ${PROJECT_SOURCE_DIR}/src/ext/stb
)

set(ext_stb_srcs
    src/ext/stb/stb_image_write.h
    src/ext/stb/stb_image.h
)

add_definitions(-DDATA_DIR=R\"\(${PROJECT_SOURCE_DIR}/../data\)\")

set(common_srcs
    src/array3D.h
    src/exceptions.h
    src/floatimage.cpp
    src/floatimage.h
    src/utils.h
)

# Compile common library
add_library(common_lib STATIC
    ${common_srcs}
    ${ext_stb_srcs}
)
SOURCE_GROUP("src\\ext\\stb" FILES ${ext_stb_srcs})

set(a6_srcs src/final_main.cpp src/gif.h src/seam.cpp src/energy.cpp src/filtering.cpp src/a6.cpp src/morphing.cpp src/a2.cpp src/server.cpp src/server.h)       # a6
add_executable(a6 ${a6_srcs})                                               # a6
target_link_libraries(a6 common_lib)                                        # a6
SOURCE_GROUP("src" FILES ${a6_srcs})                                        # a6

set(seam_carving_srcs src/main.cpp src/gif.h src/seam.cpp src/energy.cpp src/filtering.cpp src/a6.cpp src/morphing.cpp src/a2.cpp)
add_executable(seam_carving ${seam_carving_srcs})
target_link_libraries(seam_carving common_lib)
SOURCE_GROUP("src" FILES ${seam_carving_srcs})

# Create the output data directory
add_custom_command(TARGET a6 POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/data/output") # a6
