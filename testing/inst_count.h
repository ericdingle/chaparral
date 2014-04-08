#ifndef BONAVISTA_TESTING_INSTANCE_COUNT_H_
#define BONAVISTA_TESTING_INSTANCE_COUNT_H_

#include "bonavista/base/macros.h"
#include "bonavista/base/types.h"

class InstCount {
 public:
  InstCount();
  ~InstCount();

  static uint inst_count() { return inst_count_; }

 private:
  static uint inst_count_;

  DISALLOW_COPY_AND_ASSIGN(InstCount);
};

template <typename T>
class InstCountT {
 public:
  InstCountT() { ++inst_count_; }
  ~InstCountT() { --inst_count_; }

  static uint inst_count() { return inst_count_; }

 private:
  static uint inst_count_;

  DISALLOW_COPY_AND_ASSIGN(InstCountT);
};

template <typename T>
uint InstCountT<T>::inst_count_ = 0;

#endif
