#include "executer/any.h"

Any::Any(const Any& any) {
  *this = any;
}

Any& Any::operator=(const Any& any) {
  data_ = any.data_->copy();
  return *this;
}
