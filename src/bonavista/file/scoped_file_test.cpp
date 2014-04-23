#include "bonavista/file/scoped_file.h"

#include <stdio.h>
#include "bonavista/base/macros.h"
#include "bonavista/file/util.h"
#include "bonavista/testing/test_case.h"

TEST_CASE(ScopedFileTest) {
 protected:
  void SetUp() {
    file_ = OpenFile("test.tmp", "w+b");
  }

  void TearDown() {
    remove("test.tmp");
  }

  FILE* file_;
};

TEST(ScopedFileTest, ConstructorAndDestructor) {
  {
    scoped_FILE scoped_file(file_);

    int bytes[] = {1, 2, 3, 4};
    EXPECT_EQ(4u, fwrite(bytes, sizeof(int), 4, file_));

    EXPECT_EQ(0, fseek(file_, 0, SEEK_SET));
    int buffer[ARRAY_SIZE(bytes)];
    EXPECT_EQ(4u, fread(&buffer, sizeof(int), 4, file_));

    for (uint i = 0; i < ARRAY_SIZE(bytes); ++i) {
      EXPECT_EQ(bytes[i], buffer[i]);
    };
  }

  EXPECT_NOT_EQ(0, fseek(file_, 0, SEEK_SET));
}
