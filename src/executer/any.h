#ifndef EXECUTER_ANY_H_
#define EXECUTER_ANY_H_

#include <memory>

class Any {
 public:
  template <typename T>
  explicit Any(T value) : data_(new Data<T>(value)) {}
  Any(const Any&) = delete;
  Any& operator=(const Any&) = delete;
  ~Any() = default;

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

#endif  // EXECUTER_ANY_H_
