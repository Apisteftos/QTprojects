cmake_minimum_required(VERSION 3.21)
project(qt_mvvm_demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

# Executable named 'app' to avoid clash with the qml module
# directory that qt_add_qml_module creates (also named qt_mvvm_demo)
qt_add_executable(app
    main.cpp
    taskmodel.h
    taskmodel.cpp
    taskviewmodel.h
    taskviewmodel.cpp
)

# QML module keeps URI qt_mvvm_demo — this is the QRC path
qt_add_qml_module(app
    URI qt_mvvm_demo
    VERSION 1.0
    QML_FILES
        Main.qml
        RESOURCES README.md
)

target_link_libraries(app PRIVATE Qt6::Quick)
