cmake_minimum_required(VERSION 3.21)

project(18_testing_qml VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(app18_testing_qml
    main.cpp
)

qt_add_qml_module(app18_testing_qml
    URI testing_qml
    QML_FILES
        Main.qml
        OrderForm.qml
        Stepper.qml
        logic.js
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(app18_testing_qml PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.app18_testing_qml
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(app18_testing_qml
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS app18_testing_qml
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# === Tests ===
# The tests are a separate program. It uses the QML files from the source folder.
enable_testing()
find_package(Qt6 REQUIRED COMPONENTS QuickTest)

qt_add_executable(tst_testing_qml tests/tst_main.cpp)
target_compile_definitions(tst_testing_qml PRIVATE
    QUICK_TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests"
)
target_link_libraries(tst_testing_qml PRIVATE Qt6::QuickTest Qt6::Quick)

add_test(NAME qml_tests COMMAND tst_testing_qml)
# Without a screen (a server, CI) the tests run with the "offscreen" platform:
set_tests_properties(qml_tests PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
