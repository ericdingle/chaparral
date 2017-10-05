#ifndef EXECUTER_VARIANT_H_
#define EXECUTER_VARIANT_H_

#include <memory>

class Variant {
 public:
  template <typename T>
  explicit Variant(T value) : data_(new Data<T>(value)) {}
  template <typename T>
  explicit Variant(const std::shared_ptr<T>& value) : data_(new PointerData<T>(value)) {}
  Variant(const Variant&) = delete;
  Variant& operator=(const Variant&) = delete;
  ~Variant();

  template <typename T>
  bool Get(T* out) const {
    const Data<T>* data = dynamic_cast<const Data<T>*>(data_.get());
    if (data) {
      *out = data->value();
      return true;
    }

    return false;
  }

  template <typename T>
  bool Get(std::shared_ptr<T>* out) const {
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
    DataBase(const DataBase&) = delete;
    DataBase& operator=(const DataBase&) = delete;
    virtual ~DataBase();
  };

  template <typename T>
  class Data : public DataBase {
   public:
    explicit Data(T t) : value_(t) {}
    Data(const Data&) = delete;
    Data& operator=(const Data&) = delete;
    virtual ~Data() {}

    const T& value() const { return value_; }

   private:
    T value_;
  };

  template <typename T>
  class PointerData : public DataBase {
   public:
    explicit PointerData(const std::shared_ptr<T> t) : value_(t) {}
    PointerData(const PointerData&) = delete;
    PointerData& operator=(const PointerData&) = delete;
    virtual ~PointerData() {}

    const std::shared_ptr<T>& value() const { return value_; }

   private:
    std::shared_ptr<T> value_;
  };

  std::unique_ptr<const DataBase> data_;
};

#endif  // EXECUTER_VARIANT_H_
