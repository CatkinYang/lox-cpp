add_test([=[ScannerTest.BasicTest1]=]  /Users/catkin/Study/compilers/build/test/test_scanner [==[--gtest_filter=ScannerTest.BasicTest1]==] --gtest_also_run_disabled_tests [==[--gtest_output=xml:/Users/catkin/Study/compilers/build/test/test_scanner.xml]==] [==[--gtest_catch_exceptions=0]==])
set_tests_properties([=[ScannerTest.BasicTest1]=]  PROPERTIES WORKING_DIRECTORY /Users/catkin/Study/compilers/build/test SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==] TIMEOUT 120)
set(  test_scanner_TESTS ScannerTest.BasicTest1)
