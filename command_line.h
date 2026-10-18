#pragma once

#include <string>
#include <vector>

#include "filter.h"

namespace imgproc {

struct CommandLine {
    std::string input_path;
    std::string output_path;
    std::vector<FilterPtr> filters;
};

class CommandLineParser {
public:
    static CommandLine Parse(int argc, char* argv[]);
    static std::string HelpMessage(const std::string& program_name);
};

}  // namespace imgproc
