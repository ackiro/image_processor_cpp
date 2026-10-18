#pragma once

#include "filter.h"

namespace imgproc {

class CropFilter : public Filter {
public:
    CropFilter(int width, int height);
    void Apply(Image& image) const override;
    std::string Name() const override;

private:
    int width_ = 0;
    int height_ = 0;
};

}  // namespace imgproc
