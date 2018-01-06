cmake_minimum_required(VERSION 3.3)
project(pix)

add_library(pix_core 
    src/BrushTool
    src/LineTool
    src/RectangleTool
    src/Source
    src/Subject
    src/ToolContext
)
target_include_directories(pix_core PRIVATE /usr/include/cairo) #TODO: Put something platform independent.
target_link_libraries(pix_core PRIVATE cairo)

add_executable(pix 
    src/CommandInput
    src/Driver
    src/main
)

target_include_directories(pix 
PRIVATE 
    /usr/include/SDL2
) #TODO: Put something platform independent.
target_link_libraries(pix 
PRIVATE
    pix_core
    SDL2_ttf
    SDL2
)

add_definitions(-Wall -Werror)
