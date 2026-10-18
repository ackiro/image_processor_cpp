#pragma once

#include "filter.h"

namespace imgproc {

class GrayscaleFilter : public PointwiseFilter {
public:
    std::string Name() const override;

protected:
    Pixel Transform(const Pixel& pixel) const override;
};

}  // namespace imgproc
