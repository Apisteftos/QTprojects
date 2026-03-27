cmake_minimum_required(VERSION 3.16)
project(qt_test_demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    Sql
    Test        # QTest framework
)

qt_standard_project_setup()

qt_add_executable(qt_test_demo
    main.cpp
    # Classes under test (copied from qt_sql_demo)
    noterepository.h
    noterepository.cpp
    notemodel.h
    notemodel.cpp
    # Test suites (header-only)
    test_noterepository.h
    test_notemodel.h
)

target_link_libraries(qt_test_demo PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Sql
    Qt6::Test       # enables QTEST_MAIN, QSignalSpy, etc.
)

# Register as a CTest test so "ctest" runs it automatically
include(CTest)
add_test(NAME qt_test_demo COMMAND qt_test_demo)
