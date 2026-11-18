cmake_minimum_required(VERSION 3.21)
project(qt_http_demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

qt_standard_project_setup()

qt_add_executable(qt_http_demo
    main.cpp
    mainwindow.h
    mainwindow.cpp
    apiclient.h
    apiclient.cpp
    README.md
)

target_link_libraries(qt_http_demo PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Network    # QNetworkAccessManager, QNetworkReply
)
