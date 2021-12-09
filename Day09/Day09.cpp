#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <utility>
#include <array>

struct FloorPoint {
    long x = 0;
    long y = 0;
    long val = 0;

    std::string str() {
        std::string result = "X" + std::to_string(x) + "Y" + std::to_string(y);
        return result;
    }
};

struct Basin {
    long count = 0;
    std::unordered_set<std::string> area;
};

bool check_coord(long x, long y, std::unordered_map<std::string, FloorPoint>& data, FloorPoint& comp) {
    FloorPoint temp;
    temp.x = x + comp.x;
    temp.y = y + comp.y;
    if (data.find(temp.str()) == data.end())
        return true;
    return comp.val < data[temp.str()].val;
}

long check_coord(long x, long y, std::unordered_map<std::string, FloorPoint>& data) {
    FloorPoint temp;
    temp.x = x;
    temp.y = y;
    if (data.find(temp.str()) == data.end())
        return 10;
    return data[temp.str()].val;
}

std::unordered_map<std::string, FloorPoint> load_input(std::string filename) {
    std::ifstream input(filename);
    std::unordered_map<std::string, FloorPoint> output;
    long y = 0;
    for (std::string i; std::getline(input, i);) {
        for (long x = 0; x < i.size(); x++) {
            FloorPoint temp;
            temp.x = x;
            temp.y = y;
            temp.val = i[x] - '0';
            output[temp.str()] = temp;
        }
        y++;
    }
    return output;
}

long part_one(std::unordered_map<std::string, FloorPoint>& data, std::vector<FloorPoint>& floors) {
    long result = 0;
    for (auto& i : data) {
        bool up = check_coord(0, -1, data, i.second);
        bool down = check_coord(0, 1, data, i.second);
        bool left = check_coord(-1, 0, data, i.second);
        bool right = check_coord(1, 0, data, i.second);

        if (up && down && left && right) {
            result += i.second.val + 1;
            floors.push_back(i.second);
        }
    }
    return result;
}

void transverse(long x, long y, std::unordered_map<std::string, FloorPoint>& data, Basin& basin) {
    FloorPoint curr;
    curr.x = x;
    curr.y = y;

    if (basin.area.find(curr.str()) != basin.area.end())
        return;

    basin.area.insert(curr.str());
    if (check_coord(x, y, data) >= 9) {
        return;
    }
    basin.count++;

    transverse(x + 1, y, data, basin);
    transverse(x - 1, y, data, basin);
    transverse(x, y + 1, data, basin);
    transverse(x, y - 1, data, basin);
}

long part_two(std::unordered_map<std::string, FloorPoint>& data, std::vector<FloorPoint>& floors) {
    std::vector<long> basin_list;
    for (auto& i : floors) {
        Basin basin;
        transverse(i.x, i.y, data, basin);
        basin_list.push_back(basin.count);
    }
    std::sort(basin_list.begin(), basin_list.end());
    long end = basin_list.size() - 1;
    return basin_list[end] * basin_list[end - 1] * basin_list[end - 2];
}

int main()
{
    auto test = load_input("example.txt");
    auto input = load_input("day09.txt");

    std::vector<FloorPoint> test_floor;
    std::vector<FloorPoint> input_floor;

    long p1_test = part_one(test, test_floor);
    long p1_input = part_one(input, input_floor);
    std::cout << "Part 1: Test " << p1_test << ", Output " << p1_input << "\n";

    long p2_test = part_two(test, test_floor);
    long p2_input = part_two(input, input_floor);
    std::cout << "Part 2: Test " << p2_test << ", Output " << p2_input << "\n";
}