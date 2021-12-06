#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

std::vector<long long> read_data(std::string file) {
    std::ifstream input(file);
    std::vector<long long> output;
    for (std::string i; std::getline(input, i, ','); ) {
        output.push_back(std::stol(i));
    }
    return output;
}

void update(std::vector<long long>& data) {
    long long new_fish = data[0];
    for (int i = 1; i <= 8; i++) {
        data[i - 1] = data[i];
    }
    data[6] += new_fish;
    data[8] = new_fish;
}

std::string count_fish(std::vector<long long>& data) {
    long long fish = 0;
    for (auto& i : data) {
        fish += i;
    }
    return std::to_string(fish);
}

int main()
{
    //std::string filename = "example.txt";
    std::string filename = "day06.txt";

    std::vector<long long> data = read_data(filename);
    std::vector<long long> fish;
    for (int i = 0; i <= 8; i++) {
        fish.push_back(0);
    }

    for (auto i : data) {
        fish[i]++;
    }

    std::vector<long long> fresh = fish;

    for (long i = 0; i < 80; i++) {
        update(fish);
    }
    std::cout << "Test result day 80 = " << count_fish(fish) << "\n";
    
    fish.clear();
    fish = fresh;

    for (long i = 0; i < 256; i++) {
        update(fish);
    }
    std::cout << "Test result day 256 = " << count_fish(fish) << "\n";
}