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

target_link_libraries(WeatherViewerQt  PRIVATE
        Qt6::Widgets
)
