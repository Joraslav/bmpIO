#pragma once

#include "BMP.hpp"

#include <fstream>
#include <string>
#include <vector>

namespace bmp
{
    class BMPProcessor
    {
    private:
        int width_ = 0;
        int height_ = 0;
        int bits_per_pixel_ = 0;
        std::vector<Pixel> pixels_;
        BITMAPFILEHEADER file_header_;
        BITMAPINFOHEADER info_header_;

        void ReadHeaders(std::ifstream &file);
        void ReadPixels(std::ifstream &file);
        void WriteHeaders(std::ofstream &file) const;
        void WritePixels(std::ofstream &file) const;

        int GetRowSize() const;

    public:
        BMPProcessor() = default;
        ~BMPProcessor() = default;

        void Load(const std::string &filename);
        void Save(const std::string &filename) const;
        void Display() const;
        void DrawLine(int x1, int y1, int x2, int y2);
        void DrawCross();
    };

} // namespace bmp
