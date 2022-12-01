#include <fcsv.hpp>

#include <fstream>
#include <iostream>

#include <chrono>

using namespace std::chrono_literals;


auto main() -> int
{
    std::ifstream file{"test.csv"};

    fcsv2::Reader<int, int, int, int, int> reader{file};

    const auto start = std::chrono::high_resolution_clock::now();


    for (auto i = 0; i < 1000000; ++i)
    {
        auto t = reader.next();
        (void) t;
    }

    const auto stop = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << duration.count() << "us" << std::endl;

    return 0;
}

















/*


    const auto start = std::chrono::high_resolution_clock::now();
    const auto records = fcsv::read<std::string, std::string, std::string, std::string>(file);
    const auto stop = std::chrono::high_resolution_clock::now();

    const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << duration.count() << "us" << std::endl;



*/