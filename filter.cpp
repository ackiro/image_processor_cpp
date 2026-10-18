#include "filter.h"

#include <stdexcept>

namespace imgproc {

void PointwiseFilter::Apply(Image& image) const {
    for (int y = 0; y < image.Height(); ++y) {
        for (int x = 0; x < image.Width(); ++x) {
            Pixel transformed = Transform(image.At(x, y));
            transformed.Clamp();
            image.Set(x, y, transformed);
        }
    }
}

MatrixFilter::MatrixFilter(std::vector<std::vector<double>> kernel) : kernel_(std::move(kernel)) {
    if (kernel_.empty() || kernel_[0].empty()) {
        throw std::invalid_argument("convolution kernel must not be empty");
    }

    const std::size_t expected_width = kernel_[0].size();
    if (expected_width % 2U == 0U || kernel_.size() % 2U == 0U) {
        throw std::invalid_argument("convolution kernel dimensions must be odd");
    }

    for (const std::vector<double>& row : kernel_) {
        if (row.size() != expected_width) {
            throw std::invalid_argument("convolution kernel must be rectangular");
        }
    }

    radius_x_ = static_cast<int>(expected_width / 2U);
    radius_y_ = static_cast<int>(kernel_.size() / 2U);
}

void MatrixFilter::Apply(Image& image) const {
    Image result(image.Width(), image.Height());

    for (int y = 0; y < image.Height(); ++y) {
        for (int x = 0; x < image.Width(); ++x) {
            Pixel sum;
            for (int kernel_y = -radius_y_; kernel_y <= radius_y_; ++kernel_y) {
                for (int kernel_x = -radius_x_; kernel_x <= radius_x_; ++kernel_x) {
                    const Pixel& sample = image.AtClamped(x + kernel_x, y + kernel_y);
                    const std::size_t row_index = static_cast<std::size_t>(kernel_y + radius_y_);
                    const std::size_t column_index = static_cast<std::size_t>(kernel_x + radius_x_);
                    const double weight = kernel_[row_index][column_index];
                    sum += sample * weight;
                }
            }
            sum.Clamp();
            result.Set(x, y, sum);
        }
    }

    image = std::move(result);
}

const std::vector<std::vector<double>>& MatrixFilter::Kernel() const noexcept {
    return kernel_;
}

int MatrixFilter::RadiusX() const noexcept {
    return radius_x_;
}

int MatrixFilter::RadiusY() const noexcept {
    return radius_y_;
}

}  // namespace imgproc
