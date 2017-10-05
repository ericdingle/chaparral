add_library(calc_lib calc_executer.cpp calc_lexer.cpp calc_parser.cpp)
target_link_libraries(calc_lib executer_lib lexer_lib parser_lib)

add_executable(calc main.cpp)
target_link_libraries(calc calc_lib)

add_executable(calc_test calc_executer_test.cpp calc_lexer_test.cpp calc_parser_test.cpp)
target_link_libraries(calc_test gtest_main calc_lib)
add_test(NAME calc_test COMMAND calc_test)
