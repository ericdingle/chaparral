#ifndef EXECUTER_ANY_H_
#define EXECUTER_ANY_H_

#include <memory>

class Any {
 public:
  Any() = default;
  template <typename T>
  explicit Any(const T& value) : data_(new Data<T>(value)) {}

  Any(const Any& any);
  Any(Any&&) = default;
  Any& operator=(const Any& any);

  ~Any() = default;

  template <typename T>
  bool Get(T* value) const;

 private:
  class DataBase {
   public:
    DataBase() = default;
    DataBase(const DataBase&) = delete;
    DataBase& operator=(const DataBase&) = delete;
    virtual ~DataBase() = default;

    virtual std::unique_ptr<DataBase> copy() const = 0;
  };

  template <typename T>
  class Data : public DataBase {
   public:
    explicit Data(const T& value) : value_(value) {}
    Data(const Data&) = delete;
    Data& operator=(const Data&) = delete;
    ~Data() override = default;

    std::unique_ptr<DataBase> copy() const override {
      return std::unique_ptr<DataBase>(new Data<T>(value_));
    }

    const T& value() const { return value_; }

   private:
    const T value_;
  };

  std::unique_ptr<const DataBase> data_;
};

template <typename T>
inline bool Any::Get(T* value) const {
  const Data<T>* data = dynamic_cast<const Data<T>*>(data_.get());
  if (data) {
    *value = data->value();
    return true;
  }
  return false;
}

#endif  // EXECUTER_ANY_H_
