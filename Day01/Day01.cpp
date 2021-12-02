#include <iostream>
#include <string>
#include <vector>
#include <fstream>

// Part Two
int main()
{
    std::ifstream raw_data("data01.txt");
    std::vector<long> data;

    for (std::string i; std::getline(raw_data, i); ) {
        data.push_back(std::stol(i));
    }

    long increase = 0;
    long previous = data[0] + data[1] + data[3];

    for (size_t i = 3; i < data.size(); i++) {
        long j = data[i] + data[i - 1] + data[i - 2];
        if (j > previous) increase++;
        previous = j;
    }

    std::cout << "Hasil : " << increase << "\n";
}

/* // Part One
int main()
{
    std::ifstream raw_data("data01.txt");
    std::vector<long> data;

    for (std::string i; std::getline(raw_data, i); ) {
        data.push_back(std::stol(i));
    }

    long increase = 0;

    for (long i = 1; i < data.size(); i++) {
        if (data[i] > data[i - 1]) increase++;
    }

    std::cout << "Hasil : " << increase << "\n";
}
*/