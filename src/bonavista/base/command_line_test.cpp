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
  EXPECT_EQ(3u, args.size());
  EXPECT_EQ("arg1", args[0]);
  EXPECT_EQ("-not-a-flag", args[1]);
  EXPECT_EQ("arg2", args[2]);
}

TEST(CommandLineTest, HasFlag) {
  EXPECT_TRUE(cmd_line_.HasFlag("input"));
  EXPECT_TRUE(cmd_line_.HasFlag("output"));
  EXPECT_TRUE(cmd_line_.HasFlag("enable-logging"));
}

TEST(CommandLineTest, GetFlag) {
  EXPECT_EQ("file1.txt", cmd_line_.GetFlag("input"));
  EXPECT_EQ("", cmd_line_.GetFlag("output"));
  EXPECT_EQ("", cmd_line_.GetFlag("enable-logging"));

  EXPECT_EQ("file1.txt", cmd_line_.GetFlag("input", "def"));
  EXPECT_EQ("def", cmd_line_.GetFlag("blah", "def"));
}
