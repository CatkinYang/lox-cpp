if(EXISTS "/Users/catkin/Study/compilers/build/test/test_parser[1]_tests.cmake")
  include("/Users/catkin/Study/compilers/build/test/test_parser[1]_tests.cmake")
else()
  add_test(test_parser_NOT_BUILT test_parser_NOT_BUILT)
endif()