#include "negative_filter.h"

namespace imgproc {

namespace {
constexpr double KMaxColorComponent = 1.0;
}  // namespace

Pixel NegativeFilter::Transform(const Pixel& pixel) const {
    return Pixel(KMaxColorComponent - pixel.r, KMaxColorComponent - pixel.g, KMaxColorComponent - pixel.b);
}

std::string NegativeFilter::Name() const {
    return "neg";
}

}  // namespace imgproc
