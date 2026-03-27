cmake_minimum_required(VERSION 3.16)
project(qt_thread_demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts)

qt_standard_project_setup()

qt_add_executable(qt_thread_demo
    main.cpp
    mainwindow.h
    mainwindow.cpp
    worker.h
    worker.cpp
    threadcontroller.h
    threadcontroller.cpp
)

target_link_libraries(qt_thread_demo PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Charts    # QChart, QLineSeries, QChartView
)
