add_test([=[ParserTest.BasicTest1]=]  /Users/catkin/Study/compilers/build/test/test_parser [==[--gtest_filter=ParserTest.BasicTest1]==] --gtest_also_run_disabled_tests [==[--gtest_output=xml:/Users/catkin/Study/compilers/build/test/test_parser.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties([=[ParserTest.BasicTest1]=]  PROPERTIES WORKING_DIRECTORY /Users/catkin/Study/compilers/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 120)
set(  test_parser_TESTS ParserTest.BasicTest1)
