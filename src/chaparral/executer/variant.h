#ifndef CHAPARRAL_EXECUTER_VARIANT_H_
#define CHAPARRAL_EXECUTER_VARIANT_H_

#include <memory>
#include "bonavista/base/macros.h"

using std::shared_ptr;

class Variant {
 public:
  template <typename T>
  explicit Variant(T value) : data_(new Data<T>(value)) {}
  template <typename T>
  explicit Variant(const shared_ptr<T>& value) : data_(new PointerData<T>(value)) {}
  ~Variant();

  template <typename T>
  bool Get(T* out) const {
    DCHECK(out);

    const Data<T>* data = dynamic_cast<const Data<T>*>(data_.get());
    if (data) {
      *out = data->value();
      return true;
    }

    return false;
  }

  template <typename T>
  bool Get(shared_ptr<T>* out) const {
    DCHECK(out);

    const PointerData<T>* data = dynamic_cast<const PointerData<T>*>(data_.get());
    if (data) {
      *out = data->value();
      return true;
    }

    return false;
  }

 private:
  class DataBase {
   public:
    DataBase();
    virtual ~DataBase();

   private:
    DISALLOW_COPY_AND_ASSIGN(DataBase);
  };

  template <typename T>
  class Data : public DataBase {
   public:
    explicit Data(T t) : value_(t) {}
    virtual ~Data() {}

    const T& value() const { return value_; }

   private:
    T value_;

    DISALLOW_COPY_AND_ASSIGN(Data);
  };

  template <typename T>
  class PointerData : public DataBase {
   public:
    explicit PointerData(const shared_ptr<T> t) : value_(t) {}
    virtual ~PointerData() {}

    const shared_ptr<T>& value() const { return value_; }

   private:
    shared_ptr<T> value_;

    DISALLOW_COPY_AND_ASSIGN(PointerData);
  };

  std::unique_ptr<const DataBase> data_;

  DISALLOW_COPY_AND_ASSIGN(Variant);
};

#endif
