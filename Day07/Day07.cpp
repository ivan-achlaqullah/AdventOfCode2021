#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <utility>

std::vector<long> read_data(std::string file) {
    std::ifstream input(file);
    std::vector<long> output;
    for (std::string i; std::getline(input, i, ','); ) {
        output.push_back(std::stol(i));
    }
    return output;
}

long fuel_used(long distance, bool constant_rate) {
    if (constant_rate)
        return distance;
    return (distance * (distance + 1)) / 2;
}

std::pair<long, long> calculate(std::unordered_map<long, long>& crabs, bool constant_rate) {
    long max = 0;
    for (auto& i : crabs) {
        if (i.first > max) {
            max = i.first;
        }
    }

    std::unordered_map<long, long> cost_map;
    std::vector<long> cost_list;
    for (long i = 0; i <= max; i++) {
        long cost = 0;
        for (auto& j : crabs) {
            long distance = j.first - i;
            if (distance == 0)
                continue;
            else if (distance < 0) {
                distance = i - j.first;
            }
            cost += fuel_used(distance, constant_rate) * j.second;
        }
        cost_map[cost] = i;
        cost_list.push_back(cost);
    }

    std::sort(cost_list.begin(), cost_list.end());    
    return std::pair<long, long> {cost_map[cost_list[0]], cost_list[0]};
}

std::unordered_map<long, long> load_crabs(std::string filename) {
    std::vector<long> input = read_data(filename);
    std::unordered_map<long, long> result;
    for (auto i : input) {
        if (result.find(i) != result.end()) {
            result[i]++;
            continue;
        }
        result[i] = 1;
    }
    return result;
}   

int main()
{
    std::unordered_map<long, long> test_crabs = load_crabs("example.txt");
    std::unordered_map<long, long> input_crabs = load_crabs("day07.txt");

    auto p1test = calculate(test_crabs, true);
    auto p1 = calculate(input_crabs, true);

    std::cout  << "Part 1 Test Pos  : " << p1test.first << "\n";
    std::cout  << "Part 1 Test Cost : " << p1test.second << "\n";
    std::cout << "Part 1 Pos  : " << p1.first << "\n";
    std::cout << "Part 1 Cost : " << p1.second << "\n";

    auto p2test = calculate(test_crabs, false);
    auto p2 = calculate(input_crabs, false);

    std::cout << "Part 2 Test Pos  : " << p2test.first << "\n";
    std::cout << "Part 2 Test Cost : " << p2test.second << "\n";
    std::cout << "Part 2 Pos  : " << p2.first << "\n";
    std::cout << "Part 2 Cost : " << p2.second << "\n";
}
