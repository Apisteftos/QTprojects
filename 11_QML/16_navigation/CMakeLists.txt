cmake_minimum_required(VERSION 3.21)

project(16_navigation VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(app16_navigation
    main.cpp
)

qt_add_qml_module(app16_navigation
    URI navigation
    QML_FILES
        Main.qml
SOURCES main.cpp
QML_FILES DetailPage.qml HomePage.qml TabsPage.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(app16_navigation PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.app16_navigation
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(app16_navigation
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS app16_navigation
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
