cmake_minimum_required(VERSION 3.16)
project(qt_sql_demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Sql module — QSqlDatabase, QSqlQuery, QSqlError
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql)

qt_standard_project_setup()

qt_add_executable(qt_sql_demo
    main.cpp
    mainwindow.h
    mainwindow.cpp
    noterepository.h
    noterepository.cpp
    notemodel.h
    notemodel.cpp
    noteview.h
    noteview.cpp
    notepresenter.h
    notepresenter.cpp
)

target_link_libraries(qt_sql_demo PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Sql
)
