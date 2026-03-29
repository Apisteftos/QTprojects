cmake_minimum_required(VERSION 3.16)
project(qt_cicd_demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(qt_cicd_demo
    main.cpp
    mainwindow.h
    mainwindow.cpp
    calculator.h
    calculator.cpp
    tests/test_calculator.h tests/README.md tests/main.cpp tests/CMakeLists.txt
)

target_link_libraries(qt_cicd_demo PRIVATE
    Qt6::Core
    Qt6::Widgets
)

# Include tests as a subdirectory
# CI builds this separately to avoid Widgets dependency in tests
# add_subdirectory(tests)
