#include "bmp.h"

#include <cstdlib>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <vector>

namespace imgproc {

namespace {
constexpr uint16_t KBmpSignature = 0x4D42;
constexpr uint32_t KInfoHeaderSize = 40;
constexpr uint16_t KSupportedBitCount = 24;
constexpr uint16_t KSupportedPlanes = 1;
constexpr uint32_t KNoCompression = 0;
constexpr uint32_t KPixelBytes = 3;
constexpr uint32_t KRowAlignment = 4;
constexpr uint32_t KCombinedHeaderSize = sizeof(BmpFileHeader) + sizeof(BmpInfoHeader);

std::size_t ComputeRowSize(int width) {
    const std::uint64_t raw_row_size = static_cast<std::uint64_t>(width) * KPixelBytes;
    return static_cast<std::size_t>((raw_row_size + (KRowAlignment - 1U)) & ~(KRowAlignment - 1U));
}

void ValidateLoadHeaders(const BmpFileHeader& file_header, const BmpInfoHeader& info_header) {
    if (file_header.signature != KBmpSignature) {
        throw std::runtime_error("invalid BMP signature");
    }
    if (info_header.header_size != KInfoHeaderSize) {
        throw std::runtime_error("only BITMAPINFOHEADER is supported");
    }
    if (info_header.planes != KSupportedPlanes) {
        throw std::runtime_error("BMP file must contain exactly one color plane");
    }
    if (info_header.bit_count != KSupportedBitCount) {
        throw std::runtime_error("only 24-bit BMP files are supported");
    }
    if (info_header.compression != KNoCompression) {
        throw std::runtime_error("compressed BMP files are not supported");
    }
    if (info_header.width <= 0) {
        throw std::runtime_error("BMP width must be positive");
    }
    if (info_header.height == 0) {
        throw std::runtime_error("BMP height must be non-zero");
    }
    if (file_header.data_offset < KCombinedHeaderSize) {
        throw std::runtime_error("BMP pixel data offset is invalid");
    }
}

}  // namespace

Image BmpCodec::Load(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        throw std::runtime_error("cannot open input BMP file: " + path);
    }

    BmpFileHeader file_header;
    BmpInfoHeader info_header;
    if (!input.read(reinterpret_cast<char*>(&file_header), sizeof(file_header))) {
        throw std::runtime_error("failed to read BMP file header");
    }
    if (!input.read(reinterpret_cast<char*>(&info_header), sizeof(info_header))) {
        throw std::runtime_error("failed to read BMP info header");
    }

    ValidateLoadHeaders(file_header, info_header);

    const int width = info_header.width;
    const int height = std::abs(info_header.height);
    const bool is_bottom_up_storage = info_header.height > 0;
    const std::size_t row_size = ComputeRowSize(width);

    Image image(width, height);
    input.seekg(static_cast<std::streamoff>(file_header.data_offset), std::ios::beg);
    if (!input) {
        throw std::runtime_error("failed to seek to BMP pixel data");
    }

    std::vector<uint8_t> row_buffer(row_size);
    for (int source_y = 0; source_y < height; ++source_y) {
        if (!input.read(reinterpret_cast<char*>(row_buffer.data()), static_cast<std::streamsize>(row_size))) {
            throw std::runtime_error("unexpected end of file while reading BMP pixel data");
        }
        const int target_y = is_bottom_up_storage ? (height - 1 - source_y) : source_y;
        for (int x = 0; x < width; ++x) {
            const std::size_t pixel_offset = static_cast<std::size_t>(x) * KPixelBytes;
            const uint8_t blue = row_buffer[pixel_offset + 0U];
            const uint8_t green = row_buffer[pixel_offset + 1U];
            const uint8_t red = row_buffer[pixel_offset + 2U];
            image.Set(x, target_y, Pixel::FromBytes(red, green, blue));
        }
    }

    return image;
}

void BmpCodec::Save(const Image& image, const std::string& path) {
    if (image.Empty()) {
        throw std::runtime_error("cannot save an empty image");
    }

    std::ofstream output(path, std::ios::binary);
    if (!output) {
        throw std::runtime_error("cannot open output BMP file: " + path);
    }

    const int width = image.Width();
    const int height = image.Height();
    const std::size_t row_size = ComputeRowSize(width);
    const std::uint64_t image_size = static_cast<std::uint64_t>(row_size) * static_cast<std::uint64_t>(height);
    const std::uint64_t file_size = static_cast<std::uint64_t>(KCombinedHeaderSize) + image_size;

    if (image_size > std::numeric_limits<uint32_t>::max() || file_size > std::numeric_limits<uint32_t>::max()) {
        throw std::runtime_error("image is too large for 24-bit BMP");
    }

    BmpFileHeader file_header;
    file_header.signature = KBmpSignature;
    file_header.file_size = static_cast<uint32_t>(file_size);
    file_header.data_offset = KCombinedHeaderSize;

    BmpInfoHeader info_header;
    info_header.header_size = KInfoHeaderSize;
    info_header.width = width;
    info_header.height = height;
    info_header.planes = KSupportedPlanes;
    info_header.bit_count = KSupportedBitCount;
    info_header.compression = KNoCompression;
    info_header.image_size = static_cast<uint32_t>(image_size);

    if (!output.write(reinterpret_cast<const char*>(&file_header), sizeof(file_header))) {
        throw std::runtime_error("failed to write BMP file header");
    }
    if (!output.write(reinterpret_cast<const char*>(&info_header), sizeof(info_header))) {
        throw std::runtime_error("failed to write BMP info header");
    }

    std::vector<uint8_t> row_buffer(row_size, 0);
    for (int y = height - 1; y >= 0; --y) {
        std::fill(row_buffer.begin(), row_buffer.end(), 0);
        for (int x = 0; x < width; ++x) {
            const Pixel& pixel = image.At(x, y);
            const std::size_t pixel_offset = static_cast<std::size_t>(x) * KPixelBytes;
            row_buffer[pixel_offset + 0U] = Pixel::ToByte(pixel.b);
            row_buffer[pixel_offset + 1U] = Pixel::ToByte(pixel.g);
            row_buffer[pixel_offset + 2U] = Pixel::ToByte(pixel.r);
        }
        if (!output.write(reinterpret_cast<const char*>(row_buffer.data()), static_cast<std::streamsize>(row_size))) {
            throw std::runtime_error("failed to write BMP pixel data");
        }
    }
}

}  // namespace imgproc
