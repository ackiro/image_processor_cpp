#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace imgproc {

struct Pixel {
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;

    Pixel() = default;
    Pixel(double red, double green, double blue) noexcept;

    void Clamp() noexcept;

    static Pixel FromBytes(uint8_t red, uint8_t green, uint8_t blue) noexcept;
    static uint8_t ToByte(double component) noexcept;

    Pixel& operator+=(const Pixel& other) noexcept;
    Pixel& operator*=(double value) noexcept;
};

Pixel operator+(Pixel left, const Pixel& right) noexcept;
Pixel operator*(Pixel pixel, double value) noexcept;
Pixel operator*(double value, Pixel pixel) noexcept;

}  // namespace imgproc
