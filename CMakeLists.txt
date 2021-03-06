cmake_minimum_required(VERSION 3.3)
project(osmMapper)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -march=native")

set(SOURCE_FILES
        main.cpp
        osmParser.cpp
        osmParser.h
        osmRoad.cpp
        osmRoad.h
        osmNode.cpp
        osmNode.h
        OSMTypes.h
        osmBounds.cpp
        osmBounds.h
        osmData.cpp
        osmData.h
        display.h
        mapRenderer.cpp
        mapRenderer.h
        xDisplay.cpp
        xDisplay.h)

add_executable(osmMapper ${SOURCE_FILES})

find_package(Boost 1.54 COMPONENTS system timer REQUIRED)
message(status ": Boost Include: ${Boost_INCLUDE_DIR}")
message(status ": Boost Libraries Dir: ${Boost_LIBRARY_DIRS}")
message(status ": Boost Libraries: ${Boost_LIBRARIES}")

add_library(boost STATIC IMPORTED)
set_property(TARGET boost PROPERTY IMPORTED_LOCATION ${Boost_LIBRARIES})
include_directories(${Boost_INCLUDE_DIR})

add_library(cairo STATIC IMPORTED)
set_property(TARGET cairo PROPERTY IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libcairo.so)
include_directories(/usr/include/cairo /usr/include/freetype2)

add_library(cairomm STATIC IMPORTED)
set_property(TARGET cairomm PROPERTY IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libcairomm-1.0.so)
include_directories(/usr/include/cairomm-1.0
        /usr/include/cairomm-1.0/cairomm
        /usr/lib/x86_64-linux-gnu/cairomm-1.0/include
        /usr/include/sigc++-2.0
        /usr/lib/x86_64-linux-gnu/sigc++-2.0/include/)

add_library(x11 STATIC IMPORTED)
set_property(TARGET x11 PROPERTY IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libX11.so)
include_directories(/usr/include/X11)

add_library(pugixml STATIC IMPORTED)
set_property(TARGET pugixml PROPERTY IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libpugixml.so)
include_directories(/usr/include/pugixml)

target_link_libraries(osmMapper ${Boost_LIBRARIES} cairo x11 cairomm pugixml pthread rt)
link_directories(${Boost_LIBRARY_DIRS})
