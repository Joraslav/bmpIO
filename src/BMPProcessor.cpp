#include "BMPProcessor.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>

namespace bmp
{
    using namespace std::literals;

    void BMPProcessor::ReadHeaders(std::ifstream &file)
    {
        file.read(reinterpret_cast<char *>(&file_header_), sizeof(file_header_));
        file.read(reinterpret_cast<char *>(&info_header_), sizeof(info_header_));

        if (file_header_.bf_type != 0x4d42)
        {
            throw std::runtime_error("Invalid file type"s);
        }

        width_ = static_cast<int>(info_header_.bi_width);
        height_ = std::abs(static_cast<int>(info_header_.bi_height));
        bits_per_pixel_ = static_cast<int>(info_header_.bi_bit_count);

        if (bits_per_pixel_ != 24 && bits_per_pixel_ != 32)
        {
            throw std::runtime_error("Only 24-bit and 32-bit BMP are supported"s);
        }
    }

    void BMPProcessor::ReadPixels(std::ifstream &file)
    {
        pixels_.resize(width_ * height_);
        file.seekg(file_header_.bf_off_bits, std::ios::beg);

        const int bytes_per_pixel = bits_per_pixel_ / 8;
        const int row_size = GetRowSize();
        std::vector<uint8_t> row(row_size);

        for (int y = 0; y < height_; ++y)
        {
            file.read(reinterpret_cast<char *>(row.data()), row_size);
            for (int x = 0; x < width_; ++x)
            {
                Pixel &p = pixels_[y * width_ + x];
                p.blue = row[x * bytes_per_pixel + 0];
                p.green = row[x * bytes_per_pixel + 1];
                p.red = row[x * bytes_per_pixel + 2];
                if (bytes_per_pixel == 4)
                {
                    p.a = row[x * bytes_per_pixel + 3];
                }

                const bool is_black = (p.red == 0 && p.green == 0 && p.blue == 0);
                const bool is_white = (p.red == 255 && p.green == 255 && p.blue == 255);
                if (!is_black && !is_white)
                {
                    throw std::runtime_error("Image contains colors other than black and white"s);
                }
            }
        }
    }

    void BMPProcessor::WriteHeaders(std::ofstream &file) const
    {
        file.write(reinterpret_cast<const char *>(&file_header_), sizeof(file_header_));
        file.write(reinterpret_cast<const char *>(&info_header_), sizeof(info_header_));
    }

    void BMPProcessor::WritePixels(std::ofstream &file) const
    {
        const int bytes_per_pixel = bits_per_pixel_ / 8;
        const int row_size = GetRowSize();
        std::vector<uint8_t> row(row_size);

        for (int y = 0; y < height_; ++y)
        {
            for (int x = 0; x < width_; ++x)
            {
                const Pixel &p = pixels_[y * width_ + x];
                row[x * bytes_per_pixel + 0] = p.blue;
                row[x * bytes_per_pixel + 1] = p.green;
                row[x * bytes_per_pixel + 2] = p.red;
                if (bytes_per_pixel == 4)
                {
                    row[x * bytes_per_pixel + 3] = p.a;
                }
            }
            file.write(reinterpret_cast<const char *>(row.data()), row_size);
        }
    }

    int BMPProcessor::GetRowSize() const
    {
        return (width_ * (bits_per_pixel_ / 8) + 3) & ~3;
    }

    void BMPProcessor::Load(const std::string &filename)
    {
        std::ifstream file(filename, std::ios::binary);
        if (!file)
        {
            throw std::runtime_error("Failed to open file: "s + filename);
        }

        ReadHeaders(file);
        ReadPixels(file);
        file.close();
    }

    void BMPProcessor::Save(const std::string &filename) const
    {
        std::ofstream file(filename, std::ios::binary);
        if (!file)
        {
            throw std::runtime_error("Failed to create file: "s + filename);
        }

        WriteHeaders(file);
        WritePixels(file);
        file.close();
    }

    void BMPProcessor::Display() const
    {
        for (int y = 0; y < height_; ++y)
        {
            for (int x = 0; x < width_; ++x)
            {
                const Pixel &p = pixels_[y * width_ + x];
                std::cout << (p.red == 255 && p.green == 255 && p.blue == 255 ? " " : "#");
            }
            std::cout << "\n";
        }
    }

    void BMPProcessor::DrawLine(int x1, int y1, int x2, int y2)
    {
        int dx = std::abs(x2 - x1);
        int dy = std::abs(y2 - y1);
        int sx = x1 < x2 ? 1 : -1;
        int sy = y1 < y2 ? 1 : -1;
        int err = dx - dy;

        while (true)
        {
            if (x1 >= 0 && x1 < width_ && y1 >= 0 && y1 < height_)
            {
                Pixel &p = pixels_[y1 * width_ + x1];
                if (p.red == 0 && p.green == 0 && p.blue == 0)
                {
                    p = {255, 255, 255, 0}; 
                }
                else
                {
                    p = {0, 0, 0, 0}; 
                }
            }

            if (x1 == x2 && y1 == y2)
                break;

            int e2 = 2 * err;
            if (e2 > -dy)
            {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx)
            {
                err += dx;
                y1 += sy;
            }
        }
    }

    void BMPProcessor::DrawCross()
    {
        DrawLine(0, 0, width_ - 1, height_ - 1);
        DrawLine(0, height_ - 1, width_ - 1, 0);
    }
} // namespace bmp
