#include "bonavista/testing/inst_count.h"

uint InstCount::inst_count_ = 0;

InstCount::InstCount() {
  ++inst_count_;
}

InstCount::~InstCount() {
  --inst_count_;
}
