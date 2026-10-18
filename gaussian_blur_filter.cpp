#include "gaussian_blur_filter.h"

#include <cmath>
#include <stdexcept>

namespace imgproc {

namespace {
constexpr double KPositiveSigmaLowerBound = 0.0;
constexpr double KRadiusMultiplier = 3.0;
constexpr double KGaussianDenominator = 2.0;
}  // namespace

GaussianBlurFilter::GaussianBlurFilter(double sigma) : sigma_(sigma) {
    if (sigma_ <= KPositiveSigmaLowerBound) {
        throw std::invalid_argument("blur sigma must be positive");
    }
}

std::vector<double> GaussianBlurFilter::BuildKernel() const {
    const int radius = static_cast<int>(std::ceil(KRadiusMultiplier * sigma_));
    const std::size_t kernel_size = static_cast<std::size_t>(2 * radius + 1);
    std::vector<double> kernel(kernel_size);

    const double sigma_squared = sigma_ * sigma_;
    double weight_sum = 0.0;
    for (int offset = -radius; offset <= radius; ++offset) {
        const double exponent = -(static_cast<double>(offset * offset)) / (KGaussianDenominator * sigma_squared);
        const double weight = std::exp(exponent);
        kernel[static_cast<std::size_t>(offset + radius)] = weight;
        weight_sum += weight;
    }

    for (double& weight : kernel) {
        weight /= weight_sum;
    }

    return kernel;
}

Image GaussianBlurFilter::BlurHorizontal(const Image& image, const std::vector<double>& kernel) const {
    const int radius = static_cast<int>(kernel.size() / 2U);
    Image result(image.Width(), image.Height());

    for (int y = 0; y < image.Height(); ++y) {
        for (int x = 0; x < image.Width(); ++x) {
            Pixel sum;
            for (int offset = -radius; offset <= radius; ++offset) {
                const std::size_t kernel_index = static_cast<std::size_t>(offset + radius);
                sum += image.AtClamped(x + offset, y) * kernel[kernel_index];
            }
            result.Set(x, y, sum);
        }
    }

    return result;
}

Image GaussianBlurFilter::BlurVertical(const Image& image, const std::vector<double>& kernel) const {
    const int radius = static_cast<int>(kernel.size() / 2U);
    Image result(image.Width(), image.Height());

    for (int y = 0; y < image.Height(); ++y) {
        for (int x = 0; x < image.Width(); ++x) {
            Pixel sum;
            for (int offset = -radius; offset <= radius; ++offset) {
                const std::size_t kernel_index = static_cast<std::size_t>(offset + radius);
                sum += image.AtClamped(x, y + offset) * kernel[kernel_index];
            }
            sum.Clamp();
            result.Set(x, y, sum);
        }
    }

    return result;
}

void GaussianBlurFilter::Apply(Image& image) const {
    const std::vector<double> kernel = BuildKernel();
    const Image horizontal_blurred = BlurHorizontal(image, kernel);
    image = BlurVertical(horizontal_blurred, kernel);
}

std::string GaussianBlurFilter::Name() const {
    return "blur";
}

}  // namespace imgproc
