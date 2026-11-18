cmake_minimum_required(VERSION 3.21)
project(qt_server VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Network module — QTcpServer, QTcpSocket
find_package(Qt6 REQUIRED COMPONENTS Core Network)

qt_standard_project_setup()

qt_add_executable(qt_server
    main.cpp
    tcpserver.h
    tcpserver.cpp
    clienthandler.h
    clienthandler.cpp
    protocol.h
    README.md
)

target_link_libraries(qt_server PRIVATE
    Qt6::Core
    Qt6::Network
)
