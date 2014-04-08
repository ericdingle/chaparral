#ifndef BONAVISTA_BASE_MACROS_H_
#define BONAVISTA_BASE_MACROS_H_

#include <stdio.h>
#include <stdlib.h>

// Dynamically retrieve array size.
#define ARRAY_SIZE(arr) \
    (sizeof(arr) / sizeof(arr[0]))

// Perform (debug) checks.
#define CHECK(cond) \
  do { \
    if (!(cond)) { \
      printf("Check failure at %s:%d: %s\n", __FILE__, __LINE__, #cond); \
      exit(1); \
    } \
  } while(0)

#if DEBUG
#define DCHECK CHECK
#else
#define DCHECK(cond) \
  do { \
  } while(0)
#endif

// Instantiate empty copy constructor and assignment operator.
#define DISALLOW_COPY_AND_ASSIGN(ClassName) \
    ClassName(const ClassName&); \
    void operator=(const ClassName&)

#endif
