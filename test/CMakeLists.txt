cmake_minimum_required(VERSION 3.10)

include(GoogleTest)

file(GLOB_RECURSE LOX_TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/test_*.cc")

# ##############################################################################
# MAKE TARGETS
# ##############################################################################

add_custom_target(build-tests COMMAND ${CMAKE_CTEST_COMMAND} --show-only)

# ##############################################################################
# "make XYZ_test"
# ##############################################################################
message(STATUS "LOX_TEST_SOURCES: ${LOX_TEST_SOURCES}")
foreach(lox_test_source ${LOX_TEST_SOURCES})
  # Create a human readable name.
  get_filename_component(lox_test_filename ${lox_test_source} NAME)
  string(REPLACE ".cc" "" lox_test_name ${lox_test_filename})

  # Add the test target separately and as part of "make check-tests".
  # add_executable(${lox_test_name} EXCLUDE_FROM_ALL)
  add_executable(${lox_test_name} ${lox_test_source})
  # add_backward(${lox_test_name})
  add_dependencies(build-tests ${lox_test_name})

  gtest_discover_tests(
    ${lox_test_name}
    EXTRA_ARGS --gtest_output=xml:${CMAKE_BINARY_DIR}/test/${lox_test_name}.xml
               --gtest_catch_exceptions=0 DISCOVERY_TIMEOUT 120
    PROPERTIES
    TIMEOUT 120)

  target_link_libraries(${lox_test_name} lox gtest)

  # Set test target properties and dependencies.
  set_target_properties(
    ${lox_test_name}
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
               COMMAND ${lox_test_name})
endforeach()

# add_custom_target(test_scanner COMMAND ${CMAKE_BINARY_DIR}/test/test_scanner)

# 添加一个自定义目标，用于运行所有测试 add_custom_target(check-tests)
