#ifndef EXECUTER_INVOKABLE_H_
#define EXECUTER_INVOKABLE_H_

#include <memory>
#include <string>
#include <vector>

class Variant;

class Invokable {
 public:
  enum Result {
    RESULT_OK,
    RESULT_ERR_NAME,
    RESULT_ERR_ARG_SIZE,
    RESULT_ERR_ARG_TYPE,
    RESULT_ERR_FAIL
  };

  Invokable();
  Invokable(const Invokable&) = delete;
  Invokable& operator=(const Invokable&) = delete;
  virtual ~Invokable();

  virtual Result Invoke(
      const std::string& name,
      const std::vector<std::shared_ptr<const Variant> >& args,
      std::shared_ptr<const Variant>* var) =0;
};

#endif  // EXECUTER_INVOKABLE_H_
