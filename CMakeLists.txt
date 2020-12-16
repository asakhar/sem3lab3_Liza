cmake_minimum_required(VERSION 3.12)
project(LogicGate LANGUAGES CXX)

include(cmake/StandardProjectSettings.cmake)
include(cmake/Catch.cmake)
# Link this 'library' to set the c++ standard / compile-time options requested
add_library(default INTERFACE)
target_compile_features(default INTERFACE cxx_std_20)

include(cmake/CompilerWarnings.cmake)
set_project_warnings(default)

enable_testing()
add_subdirectory(catch2)

add_executable(StaticEdition main1.cpp LogicGate.cpp)

add_executable(DynamicEdition main.cpp LogicGateDynamic.cpp)

add_executable(OperatorsEdition main1op.cpp LogicGateOperators.cpp)

add_executable(Tests test.cpp LogicGateOperators.cpp)

target_link_libraries(Tests PUBLIC Catch2::Catch2 default)
catch_discover_tests(Tests TEST_PREFIX words::)
