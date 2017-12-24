cmake_minimum_required(VERSION 3.3)

add_library(pix_core src/Subject)

add_executable(pix src/driver)
target_include_directories(pix PRIVATE /usr/include/SDL2) #TODO: Put something platform independent.
target_link_libraries(pix 
PRIVATE
    SDL2
    pix_core
)
