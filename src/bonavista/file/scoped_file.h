#ifndef BONAVISTA_FILE_SCOPED_FILE_H_
#define BONAVISTA_FILE_SCOPED_FILE_H_

#include <memory>

struct CloseFile {
  void operator()(FILE* file) const;
};

typedef std::unique_ptr<FILE, CloseFile> scoped_FILE;

#endif
