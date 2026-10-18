#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>

#include "pixel.h"

namespace imgproc {

class Image {
public:
    Image() = default;
    Image(int width, int height);

    int Width() const noexcept;
    int Height() const noexcept;
    bool Empty() const noexcept;

    Pixel& At(int x, int y);
    const Pixel& At(int x, int y) const;
    const Pixel& AtClamped(int x, int y) const noexcept;

    void Set(int x, int y, const Pixel& pixel);
    Image Cropped(int width, int height) const;

private:
    int width_ = 0;
    int height_ = 0;
    std::vector<Pixel> data_;

    std::size_t Index(int x, int y) const noexcept;
};

}  // namespace imgproc
