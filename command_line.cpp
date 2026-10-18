#include "command_line.h"

#include <stdexcept>

#include "filter_factory.h"

namespace imgproc {

namespace {
constexpr int KRequiredFileArgumentCount = 3;
constexpr int KCropArgumentCount = 2;
constexpr int KSingleArgumentCount = 1;
constexpr int KZeroArgumentCount = 0;

bool IsFilterToken(const std::string& token) {
    return !token.empty() && token.front() == '-';
}

std::vector<std::string> ConsumeFixedArgumentCount(int argc, char* argv[], int& index, int argument_count) {
    std::vector<std::string> arguments;
    for (int consumed = 0; consumed < argument_count; ++consumed) {
        if (index + 1 >= argc) {
            throw std::invalid_argument("not enough arguments for the current filter");
        }
        ++index;
        arguments.emplace_back(argv[index]);
    }
    return arguments;
}

std::vector<std::string> ConsumeOptionalSingleArgument(int argc, char* argv[], int& index) {
    if (index + 1 >= argc) {
        return {};
    }
    const std::string next_token = argv[index + 1];
    if (IsFilterToken(next_token)) {
        return {};
    }
    ++index;
    return {next_token};
}

std::vector<std::string> ConsumeArgumentsForFilter(const std::string& filter_name, int argc, char* argv[], int& index) {
    if (filter_name == "crop") {
        return ConsumeFixedArgumentCount(argc, argv, index, KCropArgumentCount);
    }
    if (filter_name == "edge" || filter_name == "blur") {
        return ConsumeFixedArgumentCount(argc, argv, index, KSingleArgumentCount);
    }
    if (filter_name == "crystallize") {
        return ConsumeOptionalSingleArgument(argc, argv, index);
    }
    if (filter_name == "gs" || filter_name == "neg" || filter_name == "sharp") {
        return ConsumeFixedArgumentCount(argc, argv, index, KZeroArgumentCount);
    }
    throw std::invalid_argument("unknown filter: -" + filter_name);
}

}  // namespace

CommandLine CommandLineParser::Parse(int argc, char* argv[]) {
    if (argc < KRequiredFileArgumentCount) {
        throw std::invalid_argument("input and output BMP files are required");
    }

    CommandLine command_line;
    command_line.input_path = argv[1];
    command_line.output_path = argv[2];

    for (int index = 3; index < argc; ++index) {
        const std::string token = argv[index];
        if (!IsFilterToken(token)) {
            throw std::invalid_argument("filter names must start with '-'");
        }
        const std::string filter_name = token.substr(1);
        const std::vector<std::string> filter_arguments = ConsumeArgumentsForFilter(filter_name, argc, argv, index);
        command_line.filters.push_back(CreateFilter(filter_name, filter_arguments));
    }

    return command_line;
}

std::string CommandLineParser::HelpMessage(const std::string& program_name) {
    return "Usage: " + program_name +
           " input.bmp output.bmp [filters...]\n\n"
           "Filters:\n"
           "  -crop width height\n"
           "  -gs\n"
           "  -neg\n"
           "  -sharp\n"
           "  -edge threshold\n"
           "  -blur sigma\n"
           "  -crystallize [cell_size]\n";
}

}  // namespace imgproc
