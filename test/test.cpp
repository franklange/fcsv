#include <fcsv.hpp>

#include <fstream>
#include <iostream>

auto main() -> int
{
    std::ifstream file{"test.csv"};

    const auto records = fcsv::read<std::string, unsigned, int, float, double>(file);

    for (const auto& r : records)
        std::cout << fcsv::to_string(r) << std::endl;

    std::cout << std::get<0>(records.at(0)) << std::endl;

    fcsv::write(records, std::cout);
    fcsv::write(records.at(1), std::cout);

    return 0;
}
