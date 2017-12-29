cmake_minimum_required(VERSION 3.3)
project(pix)

add_library(pix_core src/Subject)
target_include_directories(pix_core PRIVATE /usr/include/cairo) #TODO: Put something platform independent.
target_link_libraries(pix_core PRIVATE cairo)

find_package(KiWi REQUIRED)
# include(ExternalProject)

# set(EXTERNAL_PREFIX ${CMAKE_BINARY_DIR}/externals)
# set(KIWI_SOURCE_DIR ${EXTERNAL_PREFIX}/src/KiWi_project/src)
# set(KIWI_INSTALL_DIR ${EXTERNAL_PREFIX}/src/KiWi_project/src)

# ExternalProject_Add(
#     KiWi_project
#     PREFIX ${EXTERNAL_PREFIX}
#     GIT_REPOSITORY https://github.com/mobius3/KiWi
#     TIMEOUT 10
#     LOG_DOWNLOAD ON
#     INSTALL_DIR 
# )

# add_library(KiWi SHARED IMPORTED)
# ExternalProject_Get_Property(KiWi_project BINARY_DIR)
# set_property(TARGET KiWi PROPERTY IMPORTED_LOCATION ${BINARY_DIR}/src/libKiWi.so)
# add_dependencies(KiWi KiWi_project)

add_executable(pix src/driver)
target_include_directories(pix 
PRIVATE 
    /usr/include/SDL2
    ${KiWi_INCLUDE_DIR}
) #TODO: Put something platform independent.
target_link_libraries(pix 
PRIVATE
    ${KIWI_LIBRARY}
    pix_core
    SDL2_image
    SDL2_ttf
    SDL2
)

add_definitions(-Wall -Werror)
