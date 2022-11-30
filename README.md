# fcsv

Very basic CSV reader/writer.

```c++
#include "fcsv.hpp"

#include <fstream>
#include <iostream>

auto main() -> int
{
    std::ifstream file{"test.csv"};

    // parse as vector<tuple<...>>
    const auto records = fcsv::read<std::string, unsigned, int, float, double>(file);

    // turn records back into comma separated strings
    for (const auto& r : records)
        std::cout << fcsv::to_string(r) << std::endl;

    // access records like tuples
    std::cout << std::get<0>(records.at(0)) << std::endl;

    // write single record or all of them
    fcsv::write(records, std::cout);
    fcsv::write(records.at(1), std::cout);

    return 0;
}
```