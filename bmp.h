#pragma once

#include <cstdint>
#include <string>

#include "image.h"

namespace imgproc {

#pragma pack(push, 1)
struct BmpFileHeader {
    uint16_t signature = 0;
    uint32_t file_size = 0;
    uint16_t reserved1 = 0;
    uint16_t reserved2 = 0;
    uint32_t data_offset = 0;
};

struct BmpInfoHeader {
    uint32_t header_size = 0;
    int32_t width = 0;
    int32_t height = 0;
    uint16_t planes = 0;
    uint16_t bit_count = 0;
    uint32_t compression = 0;
    uint32_t image_size = 0;
    int32_t x_pixels_per_meter = 0;
    int32_t y_pixels_per_meter = 0;
    uint32_t colors_used = 0;
    uint32_t colors_important = 0;
};
#pragma pack(pop)

class BmpCodec {
public:
    static Image Load(const std::string& path);
    static void Save(const Image& image, const std::string& path);
};

}  // namespace imgproc
