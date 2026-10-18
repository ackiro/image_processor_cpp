#pragma once

#include <string>
#include <vector>

#include "filter.h"

namespace imgproc {

FilterPtr CreateFilter(const std::string& filter_name, const std::vector<std::string>& arguments);

}  // namespace imgproc
