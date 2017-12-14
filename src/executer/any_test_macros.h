#ifndef EXECUTER_ANY_TEST_MACROS_H_
#define EXECUTER_ANY_TEST_MACROS_H_

#define EXPECT_ANY(expr, T, value) \
  do {                             \
    const Any _any = (expr);       \
    T t;                           \
    EXPECT_TRUE(_any.Get(&t));     \
    EXPECT_EQ(value, t);           \
  } while(false)

#endif  // EXECUTER_ANY_TEST_MACROS_H_
