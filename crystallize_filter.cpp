#include "crystallize_filter.h"

#include <algorithm>
#include <stdexcept>

namespace imgproc {

CrystallizeFilter::CrystallizeFilter(int cell_size) : cell_size_(cell_size) {
    if (cell_size_ <= 0) {
        throw std::invalid_argument("crystallize cell size must be positive");
    }
}

void CrystallizeFilter::Apply(Image& image) const {
    Image result(image.Width(), image.Height());

    for (int block_y = 0; block_y < image.Height(); block_y += cell_size_) {
        for (int block_x = 0; block_x < image.Width(); block_x += cell_size_) {
            const int end_x = std::min(block_x + cell_size_, image.Width());
            const int end_y = std::min(block_y + cell_size_, image.Height());
            Pixel average_color;
            int pixel_count = 0;

            for (int y = block_y; y < end_y; ++y) {
                for (int x = block_x; x < end_x; ++x) {
                    average_color += image.At(x, y);
                    ++pixel_count;
                }
            }

            average_color *= 1.0 / static_cast<double>(pixel_count);
            average_color.Clamp();

            for (int y = block_y; y < end_y; ++y) {
                for (int x = block_x; x < end_x; ++x) {
                    result.Set(x, y, average_color);
                }
            }
        }
    }

    image = std::move(result);
}

std::string CrystallizeFilter::Name() const {
    return "crystallize";
}

}  // namespace imgproc
