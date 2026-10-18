#pragma once

#include "filter.h"

namespace imgproc {

class EdgeFilter : public MatrixFilter {
public:
    explicit EdgeFilter(double threshold);
    void Apply(Image& image) const override;
    std::string Name() const override;

private:
    double threshold_ = 0.0;
};

}  // namespace imgproc
