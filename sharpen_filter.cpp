#include "sharpen_filter.h"

namespace imgproc {

namespace {
constexpr double KMinusOne = -1.0;
constexpr double KCenterWeight = 5.0;
constexpr double KZero = 0.0;
}  // namespace

SharpenFilter::SharpenFilter()
    : MatrixFilter({{KZero, KMinusOne, KZero}, {KMinusOne, KCenterWeight, KMinusOne}, {KZero, KMinusOne, KZero}}) {
}

std::string SharpenFilter::Name() const {
    return "sharp";
}

}  // namespace imgproc
