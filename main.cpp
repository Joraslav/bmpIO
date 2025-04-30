#include "BMPProcessor.hpp"
#include <iostream>

using namespace bmp;
using namespace std::literals;

int main()
{
    try
    {
        BMPProcessor processor;

        std::string input_file;
        std::cout << "Enter input BMP file name: "s;
        std::cin >> input_file;

        processor.Load(input_file);
        std::cout << "Original image:\n";
        processor.Display();

        processor.DrawCross();
        std::cout << "\nAfter drawing cross:\n";
        processor.Display();

        std::string output_file;
        std::cout << "Enter output BMP file name: "s;
        std::cin >> output_file;

        processor.Save(output_file);
        std::cout << "Image saved to "s << output_file << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}