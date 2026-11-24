cmake_minimum_required(VERSION 3.21)

project(13_cpp_signals_to_qml VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(app13_cpp_signals_to_qml
    main.cpp
)

qt_add_qml_module(app13_cpp_signals_to_qml
    URI cpp_signals_to_qml
    QML_FILES
        Main.qml
        SOURCES sensor.cpp sensor.h
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(app13_cpp_signals_to_qml PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.app13_cpp_signals_to_qml
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(app13_cpp_signals_to_qml
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS app13_cpp_signals_to_qml
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
