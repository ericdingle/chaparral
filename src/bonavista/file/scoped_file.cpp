#include "bonavista/file/scoped_file.h"

#include <stdio.h>

void CloseFile::operator()(FILE* file) const {
  if (file)
    fclose(file);
}
