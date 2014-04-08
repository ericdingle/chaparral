#include "bonavista/base/command_line.h"

#include <string.h>
#include "bonavista/base/types.h"

const char kFlagPrefix[] = "--";
const char kFlagSeparator[] = "=";

CommandLine::CommandLine(int argc, const char* const* argv) {
  DCHECK(argc >= 1);

  program_ = argv[0];

  const uint prefix_len = strlen(kFlagPrefix);
  for (int i = 1; i < argc; ++i) {
    if (strncmp(kFlagPrefix, argv[i], prefix_len) == 0) {
      const std::string arg(argv[i]);
      const size_t separator_pos = arg.find(kFlagSeparator);

      const std::string flag(arg.substr(prefix_len, separator_pos - prefix_len));
      std::string value;
      if (separator_pos != std::string::npos)
        value = arg.substr(separator_pos + 1);

      flags_[flag] = value;
    } else {
      args_.push_back(argv[i]);
    }
  }
}

CommandLine::~CommandLine() {
}

bool CommandLine::HasFlag(const std::string& flag) {
  return flags_.find(flag) != flags_.end();
}

std::string CommandLine::GetFlag(const std::string& flag) {
  return GetFlag(flag, "");
}

std::string CommandLine::GetFlag(const std::string& flag,
                                 const std::string& def) {
  FlagMap::const_iterator pair = flags_.find(flag);
  return pair == flags_.end() ? def : pair->second;
}
