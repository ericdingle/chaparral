#include "bonavista/base/command_line.h"

#include "bonavista/testing/test_case.h"

const char* args[] = {
  "prog",
  "arg1",
  "--input=file1.txt",
  "--output=",
  "--enable-logging",
  "-not-a-flag",
  "arg2"
};

TEST_CASE(CommandLineTest) {
 public:
  CommandLineTest() : cmd_line_(ARRAY_SIZE(args), args) {
  }

 protected:
  CommandLine cmd_line_;
};

TEST(CommandLineTest, Program) {
  EXPECT_EQ(cmd_line_.program(), "prog");
}

TEST(CommandLineTest, Args) {
  const std::vector<std::string>& args = cmd_line_.args();
  EXPECT_EQ(args.size(), 3u);
  EXPECT_EQ(args[0], "arg1");
  EXPECT_EQ(args[1], "-not-a-flag");
  EXPECT_EQ(args[2], "arg2");
}

TEST(CommandLineTest, HasFlag) {
  EXPECT_TRUE(cmd_line_.HasFlag("input"));
  EXPECT_TRUE(cmd_line_.HasFlag("output"));
  EXPECT_TRUE(cmd_line_.HasFlag("enable-logging"));
}

TEST(CommandLineTest, GetFlag) {
  EXPECT_EQ(cmd_line_.GetFlag("input"), "file1.txt");
  EXPECT_EQ(cmd_line_.GetFlag("output"), "");
  EXPECT_EQ(cmd_line_.GetFlag("enable-logging"), "");

  EXPECT_EQ(cmd_line_.GetFlag("input", "def"), "file1.txt");
  EXPECT_EQ(cmd_line_.GetFlag("blah", "def"), "def");
}
