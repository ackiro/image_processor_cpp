#pragma once

#include "filter.h"

namespace imgproc {

class SharpenFilter : public MatrixFilter {
public:
    SharpenFilter();
    std::string Name() const override;
};

}  // namespace imgproc
