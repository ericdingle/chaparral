#ifndef BONAVISTA_TESTING_TEST_CASE_EXPECT_
#define BONAVISTA_TESTING_TEST_CASE_EXPECT_

#include <iostream>

#define EXPECT_EQ(expected, actual) \
  do { \
    const auto expected_value = expected; \
    const auto actual_value = actual; \
    if (expected_value != actual_value) { \
      std::cout << "   Actual value of: " #actual "\n"; \
      std::cout << "                is: " << actual_value << "\n"; \
      std::cout << "but expected value: " #expected "\n"; \
      std::cout << "                is: " << expected_value << "\n"; \
      set_succeeded(false); \
      return; \
    } \
  } while(0)

#define EXPECT_NOT_EQ(expected, actual) \
  do { \
    const auto expected_value = expected; \
    const auto actual_value = actual; \
    if (expected_value == actual_value) { \
      std::cout << "Actual value of: " #actual "\n"; \
      std::cout << "  should not be: " << actual_value << "\n"; \
      set_succeeded(false); \
      return; \
    } \
  } while(0)

#define EXPECT_TRUE(actual) EXPECT_EQ(true, actual)

#define EXPECT_FALSE(actual) EXPECT_EQ(false, actual);

#define EXPECT_NULL(actual) EXPECT_EQ(0, actual);

#define EXPECT_NOT_NULL(actual) EXPECT_NOT_EQ(0, actual);

#endif
