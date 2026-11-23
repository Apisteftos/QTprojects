
# The C++ part of Workbench lives in Workbench_appContent (core/ and backend/).
option(WORKBENCH_BUILD_TESTS "Build the tests of the core library" OFF)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../Workbench_appContent/core ${CMAKE_CURRENT_BINARY_DIR}/core)

target_sources(${CMAKE_PROJECT_NAME} PUBLIC
    "main.cpp"
    "../Workbench_appContent/backend/workbench_backend.h"
    "../Workbench_appContent/backend/workbench_backend.cpp"
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../Workbench_appContent/backend
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Qml
    workbench_core
)
