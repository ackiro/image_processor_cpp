#pragma once

#include <memory>
#include <string>
#include <vector>

#include "image.h"

namespace imgproc {

class Filter {
public:
    virtual ~Filter() = default;
    virtual void Apply(Image& image) const = 0;
    virtual std::string Name() const = 0;
};

class PointwiseFilter : public Filter {
public:
    void Apply(Image& image) const override;

protected:
    virtual Pixel Transform(const Pixel& pixel) const = 0;
};

class MatrixFilter : public Filter {
public:
    explicit MatrixFilter(std::vector<std::vector<double>> kernel);
    void Apply(Image& image) const override;

protected:
    const std::vector<std::vector<double>>& Kernel() const noexcept;
    int RadiusX() const noexcept;
    int RadiusY() const noexcept;

private:
    std::vector<std::vector<double>> kernel_;
    int radius_x_ = 0;
    int radius_y_ = 0;
};

using FilterPtr = std::unique_ptr<Filter>;

}  // namespace imgproc
