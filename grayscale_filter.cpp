#include "grayscale_filter.h"

namespace imgproc {

namespace {
constexpr double KRedWeight = 0.299;
constexpr double KGreenWeight = 0.587;
constexpr double KBlueWeight = 0.114;
}  // namespace

Pixel GrayscaleFilter::Transform(const Pixel& pixel) const {
    const double grayscale_value = pixel.r * KRedWeight + pixel.g * KGreenWeight + pixel.b * KBlueWeight;
    return Pixel(grayscale_value, grayscale_value, grayscale_value);
}

std::string GrayscaleFilter::Name() const {
    return "gs";
}

}  // namespace imgproc
