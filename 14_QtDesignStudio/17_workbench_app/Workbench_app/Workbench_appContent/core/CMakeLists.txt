# The core library: plain C++23, no Qt. It can be built and tested alone.
add_library(workbench_core STATIC
    unit_converter.cpp
    color_tools.cpp
    countdown.cpp
    text_tools.cpp
    file_io.cpp
    file_kind.cpp
    recent_files.cpp
    color_tools.hpp
    countdown.hpp
    file_io.hpp
    file_kind.hpp
    recent_files.hpp
    text_tools.hpp
    unit_converter.hpp
)
target_include_directories(workbench_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(workbench_core PUBLIC cxx_std_23)

if(WORKBENCH_BUILD_TESTS)
    add_executable(core_tests core_tests.cpp)
    target_link_libraries(core_tests PRIVATE workbench_core)
    add_test(NAME core_tests COMMAND core_tests)
endif()
