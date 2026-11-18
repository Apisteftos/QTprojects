cmake_minimum_required(VERSION 3.21)
project(qt_udp_receiver VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

qt_standard_project_setup()

qt_add_executable(qt_udp_receiver
    main.cpp
    udpreceiver.h
    udpreceiver.cpp
    udpprotocol.h
)

target_link_libraries(qt_udp_receiver PRIVATE
    Qt6::Core
    Qt6::Network
)
