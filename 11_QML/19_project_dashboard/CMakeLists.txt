cmake_minimum_required(VERSION 3.21)

project(19_project_dashboard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(app19_project_dashboard
    main.cpp
)

qt_add_qml_module(app19_project_dashboard
    URI project_dashboard
    QML_FILES
        Main.qml
        MetricCard.qml
        BarChart.qml
    SOURCES
        dashboard.h
        dashboard.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(app19_project_dashboard PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.app19_project_dashboard
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(app19_project_dashboard
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS app19_project_dashboard
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
