#include "filter_factory.h"

#include <stdexcept>

#include "crop_filter.h"
#include "crystallize_filter.h"
#include "edge_filter.h"
#include "gaussian_blur_filter.h"
#include "grayscale_filter.h"
#include "negative_filter.h"
#include "sharpen_filter.h"

namespace imgproc {

namespace {
constexpr int KDefaultCrystallizeCellSize = 20;
constexpr int KTenBase = 10;

int ParseInt(const std::string& value, const std::string& parameter_name) {
    std::size_t processed_characters = 0;
    const int parsed_value = std::stoi(value, &processed_characters, KTenBase);
    if (processed_characters != value.size()) {
        throw std::invalid_argument(parameter_name + " must be an integer");
    }
    return parsed_value;
}

double ParseDouble(const std::string& value, const std::string& parameter_name) {
    std::size_t processed_characters = 0;
    const double parsed_value = std::stod(value, &processed_characters);
    if (processed_characters != value.size()) {
        throw std::invalid_argument(parameter_name + " must be a number");
    }
    return parsed_value;
}

void CheckArgumentCount(const std::string& filter_name, std::size_t actual_count, std::size_t expected_count) {
    if (actual_count != expected_count) {
        throw std::invalid_argument("filter -" + filter_name + " expects " + std::to_string(expected_count) +
                                    " argument(s)");
    }
}

}  // namespace

FilterPtr CreateFilter(const std::string& filter_name, const std::vector<std::string>& arguments) {
    if (filter_name == "crop") {
        CheckArgumentCount(filter_name, arguments.size(), 2U);
        return std::make_unique<CropFilter>(ParseInt(arguments[0], "crop width"),
                                            ParseInt(arguments[1], "crop height"));
    }
    if (filter_name == "gs") {
        CheckArgumentCount(filter_name, arguments.size(), 0U);
        return std::make_unique<GrayscaleFilter>();
    }
    if (filter_name == "neg") {
        CheckArgumentCount(filter_name, arguments.size(), 0U);
        return std::make_unique<NegativeFilter>();
    }
    if (filter_name == "sharp") {
        CheckArgumentCount(filter_name, arguments.size(), 0U);
        return std::make_unique<SharpenFilter>();
    }
    if (filter_name == "edge") {
        CheckArgumentCount(filter_name, arguments.size(), 1U);
        return std::make_unique<EdgeFilter>(ParseDouble(arguments[0], "edge threshold"));
    }
    if (filter_name == "blur") {
        CheckArgumentCount(filter_name, arguments.size(), 1U);
        return std::make_unique<GaussianBlurFilter>(ParseDouble(arguments[0], "blur sigma"));
    }
    if (filter_name == "crystallize") {
        if (arguments.empty()) {
            return std::make_unique<CrystallizeFilter>(KDefaultCrystallizeCellSize);
        }
        CheckArgumentCount(filter_name, arguments.size(), 1U);
        return std::make_unique<CrystallizeFilter>(ParseInt(arguments[0], "crystallize cell size"));
    }

    throw std::invalid_argument("unknown filter: -" + filter_name);
}

}  // namespace imgproc
