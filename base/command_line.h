#ifndef BONAVISTA_BASE_COMMAND_LINE_H_
#define BONAVISTA_BASE_COMMAND_LINE_H_

#include <map>
#include <string>
#include <vector>
#include "bonavista/base/macros.h"

class CommandLine {
 public:
  CommandLine(int argc, const char* const* argv);
  ~CommandLine();

  const std::string& program() { return program_; }
  const std::vector<std::string>& args() { return args_; }

  bool HasFlag(const std::string& flag);
  std::string GetFlag(const std::string& flag);
  std::string GetFlag(const std::string& flag, const std::string& def);

 private:
  std::string program_;
  std::vector<std::string> args_;

  typedef std::map<std::string, std::string> FlagMap;
  FlagMap flags_;

  DISALLOW_COPY_AND_ASSIGN(CommandLine);
};

#endif
