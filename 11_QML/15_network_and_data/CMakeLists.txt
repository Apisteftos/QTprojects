cmake_minimum_required(VERSION 3.21)

project(15_network_and_data VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(app15_network_and_data
    main.cpp
)

qt_add_qml_module(app15_network_and_data
    URI network_and_data
    QML_FILES
        Main.qml
        SOURCES stationclient.cpp stationclient.h
        QML_FILES data/stations.json
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(app15_network_and_data PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.app15_network_and_data
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(app15_network_and_data
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS app15_network_and_data
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
