cmake_minimum_required(VERSION 3.16)
project(qt_qml_dashboard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(app
    main.cpp
    dataprovider.h
    dataprovider.cpp
)

qt_add_qml_module(app
    URI qt_qml_dashboard
    VERSION 1.0
    QML_FILES
        Main.qml
        DashboardPage.qml
        ChartsPage.qml
        SettingsPage.qml
        AboutPage.qml
        MetricCard.qml
        ChartCanvas.qml
        RESOURCES README.md
)

target_link_libraries(app PRIVATE Qt6::Quick)
