#include "crop_filter.h"

#include <stdexcept>

namespace imgproc {

CropFilter::CropFilter(int width, int height) : width_(width), height_(height) {
    if (width_ <= 0 || height_ <= 0) {
        throw std::invalid_argument("crop width and height must be positive");
    }
}

void CropFilter::Apply(Image& image) const {
    image = image.Cropped(width_, height_);
}

std::string CropFilter::Name() const {
    return "crop";
}

}  // namespace imgproc
