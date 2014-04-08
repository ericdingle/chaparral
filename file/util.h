#ifndef BONAVISTA_FILE_UTIL_H_
#define BONAVISTA_FILE_UTIL_H_

#include <string>

FILE* OpenFile(const char* file_name, const char* mode);

bool ReadFile(const char* file_name, std::string* contents);

#endif
