add_executable(pix src/driver.cpp)
target_include_directories(pix PUBLIC /usr/include/SDL2) #TODO: Put something platform independent.
target_link_libraries(pix SDL2)
