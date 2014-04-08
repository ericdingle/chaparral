#include "bonavista/logging/string_format.h"

#include <memory>
#include <stdarg.h>
#include "bonavista/base/macros.h"

std::string StringFormat(const char* format, ...) {
  va_list args;
  va_start(args, format);

#if OS_WIN
  int len = _vscprintf(format, args);
#elif OS_LINUX
  va_list args_copy;
  va_copy(args_copy, args);
  int len = vsnprintf(NULL, 0, format, args_copy);
  va_end(args_copy);
#endif

  std::unique_ptr<char[]> buffer(new char[len + 1]);

#if OS_WIN
  int n = vsnprintf_s(buffer.get(), len + 1, len, format, args);
#elif OS_LINUX
  va_copy(args_copy, args);
  int n = vsnprintf(buffer.get(), len + 1, format, args);
  va_end(args_copy);
#endif

  DCHECK(n == len);

  va_end(args);
  return buffer.get();
}
