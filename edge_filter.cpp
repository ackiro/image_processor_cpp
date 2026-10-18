#include "edge_filter.h"

#include <stdexcept>

#include "grayscale_filter.h"

namespace imgproc {

namespace {
constexpr double KMinThreshold = 0.0;
constexpr double KMaxThreshold = 1.0;
constexpr double KMinusOne = -1.0;
constexpr double KCenterWeight = 4.0;
constexpr double KZero = 0.0;
constexpr double KWhite = 1.0;
constexpr double KBlack = 0.0;
}  // namespace

EdgeFilter::EdgeFilter(double threshold)
    : MatrixFilter({{KZero, KMinusOne, KZero}, {KMinusOne, KCenterWeight, KMinusOne}, {KZero, KMinusOne, KZero}}),
      threshold_(threshold) {
    if (threshold_ < KMinThreshold || threshold_ > KMaxThreshold) {
        throw std::invalid_argument("edge threshold must be in [0, 1]");
    }
}

void EdgeFilter::Apply(Image& image) const {
    GrayscaleFilter grayscale_filter;
    grayscale_filter.Apply(image);
    MatrixFilter::Apply(image);

    for (int y = 0; y < image.Height(); ++y) {
        for (int x = 0; x < image.Width(); ++x) {
            const double edge_value = image.At(x, y).r > threshold_ ? KWhite : KBlack;
            image.Set(x, y, Pixel(edge_value, edge_value, edge_value));
        }
    }
}

std::string EdgeFilter::Name() const {
    return "edge";
}

}  // namespace imgproc
