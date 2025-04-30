#pragma once

#include <cstdint>

#pragma pack(push, 1)
struct BITMAPFILEHEADER
{
    uint16_t bf_type;
    uint32_t bf_size;
    uint16_t bf_reserved_1;
    uint16_t bf_reserved_2;
    uint32_t bf_off_bits;
};

struct BITMAPINFOHEADER
{
    uint32_t bi_size;
    int32_t bi_width;
    int32_t bi_height;
    uint16_t bi_planes;
    uint16_t bi_bit_count;
    uint32_t bi_compression;
    uint32_t bi_size_image;
    int32_t bi_XPels_per_meter;
    int32_t bi_YPels_per_meter;
    uint32_t bi_clr_used;
    uint32_t bi_clr_important;
};
#pragma pack(pop)

struct Pixel
{
    uint8_t blue = 0;
    uint8_t green = 0;
    uint8_t red = 0;
    uint8_t a;

    Pixel() = default;
    Pixel(uint8_t b, uint8_t g, uint8_t r, uint8_t a_in)
        : blue(b), green(g), red(r), a(a_in) {}
};
