#ifndef EXECUTER_VARIANT_H_
#define EXECUTER_VARIANT_H_

#include <memory>

class Variant {
 public:
  template <typename T>
  explicit Variant(T value) : data_(new Data<T>(value)) {}
  Variant(const Variant&) = delete;
  Variant& operator=(const Variant&) = delete;
  ~Variant() = default;

  template <typename T>
  bool Get(T* out) const {
    const Data<T>* data = dynamic_cast<const Data<T>*>(data_.get());
    if (data) {
      *out = data->value();
      return true;
    }
    return false;
  }

 private:
  class DataBase {
   public:
    DataBase() = default;
    DataBase(const DataBase&) = delete;
    DataBase& operator=(const DataBase&) = delete;
    virtual ~DataBase() = default;
  };

  template <typename T>
  class Data : public DataBase {
   public:
    explicit Data(T t) : value_(t) {}
    Data(const Data&) = delete;
    Data& operator=(const Data&) = delete;
    ~Data() override = default;

    const T& value() const { return value_; }

   private:
    const T value_;
  };

  std::unique_ptr<const DataBase> data_;
};

#endif  // EXECUTER_VARIANT_H_
