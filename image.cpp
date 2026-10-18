#include "image.h"

#include <algorithm>

namespace imgproc {

Image::Image(int width, int height) : width_(width), height_(height) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("image dimensions must be positive");
    }
    const std::size_t pixel_count = static_cast<std::size_t>(width_) * static_cast<std::size_t>(height_);
    data_.assign(pixel_count, Pixel{});
}

int Image::Width() const noexcept {
    return width_;
}

int Image::Height() const noexcept {
    return height_;
}

bool Image::Empty() const noexcept {
    return width_ == 0 || height_ == 0;
}

Pixel& Image::At(int x, int y) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        throw std::out_of_range("pixel coordinates are out of range");
    }
    return data_[Index(x, y)];
}

const Pixel& Image::At(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        throw std::out_of_range("pixel coordinates are out of range");
    }
    return data_[Index(x, y)];
}

const Pixel& Image::AtClamped(int x, int y) const noexcept {
    const int clamped_x = std::clamp(x, 0, width_ - 1);
    const int clamped_y = std::clamp(y, 0, height_ - 1);
    return data_[Index(clamped_x, clamped_y)];
}

void Image::Set(int x, int y, const Pixel& pixel) {
    At(x, y) = pixel;
}

Image Image::Cropped(int width, int height) const {
    const int result_width = std::min(width, width_);
    const int result_height = std::min(height, height_);
    if (result_width <= 0 || result_height <= 0) {
        throw std::invalid_argument("crop dimensions must be positive");
    }

    Image result(result_width, result_height);
    for (int y = 0; y < result_height; ++y) {
        for (int x = 0; x < result_width; ++x) {
            result.Set(x, y, At(x, y));
        }
    }
    return result;
}

std::size_t Image::Index(int x, int y) const noexcept {
    return static_cast<std::size_t>(y) * static_cast<std::size_t>(width_) + static_cast<std::size_t>(x);
}

}  // namespace imgproc
