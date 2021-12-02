#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

int main()
{
    std::regex find_up("up (\\d+)");
    std::regex find_down("down (\\d+)");
    std::regex find_forward("forward (\\d+)");

    long x = 0;
    long y = 0;
    long aim = 0;

    std::ifstream raw_data("data02.txt");
    for (std::string i; std::getline(raw_data, i); ) {
        std::smatch match;
        if (std::regex_search(i, match, find_forward)) {
            long forward = std::stol(match[1].str());
            x += forward;
            y += aim * forward;
        }
        else if (std::regex_search(i, match, find_down)) {
            aim += std::stol(match[1].str());
        }
        else if (std::regex_search(i, match, find_up)) {
            aim -= std::stol(match[1].str());
        }
    }

    long hasil = x * y;
    std::cout << "X: " << x << " Y: " << y << " Aim: " << aim << " Hasil: " << hasil;
}