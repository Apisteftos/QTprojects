cmake_minimum_required(VERSION 3.21)
project(qt_mvc_demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    taskmodel.cpp
    taskmodel.h
    taskview.cpp
    taskview.h
    taskpresenter.cpp
    taskpresenter.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(qt_mvc_demo
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(qt_mvc_demo ${PROJECT_SOURCES})
endif()

target_link_libraries(qt_mvc_demo PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qt_mvc_demo)
endif()
