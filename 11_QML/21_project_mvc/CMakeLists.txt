cmake_minimum_required(VERSION 3.21)

project(21_project_mvc VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(app21_project_mvc
    main.cpp
)

qt_add_qml_module(app21_project_mvc
    URI project_mvc
    QML_FILES
        Main.qml
    SOURCES
        scoremodel.h
        scoremodel.cpp
        scorecontroller.h
        scorecontroller.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(app21_project_mvc PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.app21_project_mvc
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(app21_project_mvc
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS app21_project_mvc
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# === Tests: the Model and the Controller are tested WITHOUT a window and without QML ===
enable_testing()
find_package(Qt6 REQUIRED COMPONENTS Test)

qt_add_executable(tst_project_mvc
    tests/tst_score.cpp
    scoremodel.cpp
    scorecontroller.cpp
)
target_include_directories(tst_project_mvc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})   # the test is in tests/, the headers are here
target_link_libraries(tst_project_mvc PRIVATE Qt6::Quick Qt6::Test)
add_test(NAME mvc_tests COMMAND tst_project_mvc)
# The tests need no screen: the "offscreen" platform lets them run on a build server too.
set_tests_properties(mvc_tests PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
