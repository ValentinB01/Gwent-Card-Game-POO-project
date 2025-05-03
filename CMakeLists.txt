cmake_minimum_required(VERSION 3.10)
project(Gwent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

include_directories(
    include
    include/Card
    include/Core
    include/Utils
    include/GUI
    ${SFML_INCLUDE_DIR}
)

file(GLOB SOURCES
    "src/*.cpp"
    "src/Card/*.cpp"
    "src/Core/*.cpp"
    "src/Utils/*.cpp"
    "src/GUI/*.cpp"
)

add_executable(gwent ${SOURCES})

target_link_libraries(gwent 
    sfml-graphics 
    sfml-window 
    sfml-system
)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
