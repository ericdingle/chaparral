add_library(executer_lib executer.cpp executer.h invokable.cpp invokable.h variant.cpp variant.h)
target_link_libraries(executer_lib parser_lib)

if (chaparral_tests)
  add_executable(executer_test executer_test.cpp variant_test.cpp)
  target_link_libraries(executer_test gtest_main executer_lib)
  add_test(NAME executer_test COMMAND executer_test)
endif()
