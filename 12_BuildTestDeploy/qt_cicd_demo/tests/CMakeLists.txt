cmake_minimum_required(VERSION 3.21)
project(qt_cicd_demo_tests VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Test)

qt_standard_project_setup()

qt_add_executable(qt_cicd_demo_tests
    main.cpp
    ../calculator.h
    ../calculator.cpp
    test_calculator.h
)

target_include_directories(qt_cicd_demo_tests PRIVATE ..)

target_link_libraries(qt_cicd_demo_tests PRIVATE
    Qt6::Core
    Qt6::Test
)

include(CTest)
add_test(NAME CalculatorTests COMMAND qt_cicd_demo_tests)
