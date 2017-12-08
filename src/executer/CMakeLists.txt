add_library(executer_lib executer.cpp executer.h variant.h)
target_link_libraries(executer_lib parser_lib)
target_include_directories(executer_lib INTERFACE ${chaparral_SOURCE_DIR})

if (chaparral_tests)
  add_executable(executer_test executer_test.cpp variant_test.cpp)
  target_link_libraries(executer_test gtest_main executer_lib)
  add_test(NAME executer_test COMMAND executer_test)
endif()
