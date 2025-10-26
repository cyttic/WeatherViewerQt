cmake_minimum_required(VERSION 3.21)
project(WeatherViewerQt LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

# Point CMake to your Qt installation if needed:
# cmake -S . -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.7.2/msvc2022_64/lib/cmake"

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(WeatherViewerQt
    main.cpp
    MapWindow.cpp
    MapWindow.h
    LocationDialog.cpp
    LocationDialog.h
    resources.qrc
)

qt_add_resources(SOURCES resources.qrc)

target_link_libraries(WeatherViewerQt  PRIVATE
        Qt6::Widgets
)


find_package(Qt6 REQUIRED COMPONENTS Widgets Quick Positioning WebEngineWidgets)
# ... existing code ...
target_link_libraries(WeatherViewerQt PRIVATE
        Qt6::Widgets
        Qt6::Quick
        Qt6::Positioning
        Qt6::WebEngineWidgets
)

