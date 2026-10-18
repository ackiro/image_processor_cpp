#pragma once

#include <vector>

#include "filter.h"

namespace imgproc {

class GaussianBlurFilter : public Filter {
public:
    explicit GaussianBlurFilter(double sigma);
    void Apply(Image& image) const override;
    std::string Name() const override;

private:
    double sigma_ = 0.0;

    std::vector<double> BuildKernel() const;
    Image BlurHorizontal(const Image& image, const std::vector<double>& kernel) const;
    Image BlurVertical(const Image& image, const std::vector<double>& kernel) const;
};

}  // namespace imgproc
