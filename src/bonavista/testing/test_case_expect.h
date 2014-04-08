#ifndef BONAVISTA_TESTING_TEST_CASE_EXPECT_
#define BONAVISTA_TESTING_TEST_CASE_EXPECT_

#include <iostream>

#define EXPECT_EQ(value, expected) \
  do { \
    if (!((value) == (expected))) { \
      std::cout << "Expectation failure: value of " #value " is ("; \
      std::cout << (value); \
      std::cout << ") but expected value is ("; \
      std::cout << (expected); \
      std::cout << ").\n"; \
      set_succeeded(false); \
      return; \
    } \
  } while(0)

#define EXPECT_NOT_EQ(value, unexpected) \
  do { \
    if ((value) == (unexpected)) { \
      std::cout << "Expectation failure: value of " #value " is ("; \
      std::cout << (unexpected); \
      std::cout << ") but shouldn't be.\n"; \
      set_succeeded(false); \
      return; \
    } \
  } while(0)

#define EXPECT_TRUE(value) EXPECT_EQ(value, true)

#define EXPECT_FALSE(value) EXPECT_EQ(value, false);

#define EXPECT_NULL(value) EXPECT_EQ(value, 0);

#define EXPECT_NOT_NULL(value) EXPECT_NOT_EQ(value, 0);

#endif
