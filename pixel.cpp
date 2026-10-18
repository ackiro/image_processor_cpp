#include "pixel.h"

namespace imgproc {

namespace {
constexpr double KMinColorComponent = 0.0;
constexpr double KMaxColorComponent = 1.0;
constexpr double KByteScale = 255.0;
}  // namespace

Pixel::Pixel(double red, double green, double blue) noexcept : r(red), g(green), b(blue) {
}

void Pixel::Clamp() noexcept {
    r = std::clamp(r, KMinColorComponent, KMaxColorComponent);
    g = std::clamp(g, KMinColorComponent, KMaxColorComponent);
    b = std::clamp(b, KMinColorComponent, KMaxColorComponent);
}

Pixel Pixel::FromBytes(uint8_t red, uint8_t green, uint8_t blue) noexcept {
    return Pixel(static_cast<double>(red) / KByteScale, static_cast<double>(green) / KByteScale,
                 static_cast<double>(blue) / KByteScale);
}

uint8_t Pixel::ToByte(double component) noexcept {
    const double clamped = std::clamp(component, KMinColorComponent, KMaxColorComponent);
    return static_cast<uint8_t>(std::lround(clamped * KByteScale));
}

Pixel& Pixel::operator+=(const Pixel& other) noexcept {
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
}

Pixel& Pixel::operator*=(double value) noexcept {
    r *= value;
    g *= value;
    b *= value;
    return *this;
}

Pixel operator+(Pixel left, const Pixel& right) noexcept {
    left += right;
    return left;
}

Pixel operator*(Pixel pixel, double value) noexcept {
    pixel *= value;
    return pixel;
}

Pixel operator*(double value, Pixel pixel) noexcept {
    pixel *= value;
    return pixel;
}

}  // namespace imgproc
