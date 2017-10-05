add_library(executer_lib executer.cpp invokable.cpp variant.cpp)
target_link_libraries(executer_lib parser_lib)

add_executable(executer_test executer_test.cpp variant_test.cpp)
target_link_libraries(executer_test gtest_main executer_lib)
add_test(NAME executer_test COMMAND executer_test)
