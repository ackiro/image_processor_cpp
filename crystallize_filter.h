#pragma once

#include "filter.h"

namespace imgproc {

class CrystallizeFilter : public Filter {
public:
    explicit CrystallizeFilter(int cell_size);
    void Apply(Image& image) const override;
    std::string Name() const override;

private:
    int cell_size_ = 0;
};

}  // namespace imgproc
