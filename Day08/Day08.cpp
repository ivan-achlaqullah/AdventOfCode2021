#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <array>

std::pair<std::vector<std::string>, std::vector<std::string>> read_data(std::string file) {
    std::ifstream input(file);
    std::regex find_pattern("(.+)\\s\\|\\s(.+)");
    std::vector<std::string> output_left;
    std::vector<std::string> output_right;

    for (std::string i; std::getline(input, i); ) {
        std::smatch match;
        if (!std::regex_search(i, match, find_pattern)) continue;
        output_left.push_back(match[1].str());
        output_right.push_back(match[2].str());
    }

    return std::pair<std::vector<std::string>, std::vector<std::string>>(output_left, output_right);
}

std::vector<std::string> split_data(std::vector<std::string>& data) {
    std::vector<std::string> output;
    for (auto& i : data) {
        std::stringstream str_stream(i);
        for (std::string j; std::getline(str_stream, j, ' '); ) {
            output.push_back(j);
        }
    }
    return output;
}

std::vector<std::string> split_data(std::string& data) {
    std::stringstream str_stream(data);
    std::vector<std::string> output;
    for (std::string i; std::getline(str_stream, i, ' '); ) {
        output.push_back(i);
    }
    return output;
}

long part_one(std::vector<std::string>& data) {
    auto split = split_data(data);
    long count = 0;
    for (auto& i : split) {
        long num = i.size();
        bool digit = num == 2 || num == 4 || num == 3 || num == 7;
        if (digit) count++;
    }
    return count;
}

std::string generate_match(std::string input, std::unordered_map<char, char>& inter) {
    std::string output;
    for (auto& i : input) {
        if (inter.find(i) == inter.end())
            continue;
        output += inter[i];
    }
    std::sort(output.begin(), output.end());
    return output;
}

// TODO
std::unordered_map<std::string, long> parse_segment(std::string& input) {
    std::array<char, 7> segment = { 'a','b','c','d','e','f','g' };
    std::unordered_map<char, char> inter;

    // 2. Find char with count of 4,6,9 (E,B,F)
    for (auto i : segment) {
        long count = std::count(input.begin(), input.end(), i);
        if (count == 4) {
            inter[i] = 'e';
        }
        else if (count == 6) {
            inter[i] = 'b';
        }
        else if (count == 9) {
            inter[i] = 'f';
        }
    }

    //3. Find word with 2 chars, add the non - F char to C
    auto split = split_data(input);
    for (auto& i : split) {
        if (i.size() != 2)
            continue;
        if (inter.find(i[0]) != inter.end()){
            inter[i[1]] = 'c';
        }
        else {
            inter[i[0]] = 'c';
        }
        break;
    }

    //4. Find word with 3 chars, add the non-CF char to A
    for (auto& i : split) {
        if (i.size() != 3)
            continue;
        for (int j = 0; j < 3; j++) {
            if (inter.find(i[j]) != inter.end())
                continue;
            inter[i[j]] = 'a';
            break;
        }
        break;
    }

    //5. Find word with 4 chars, add the non-BCF char to D
    for (auto& i : split) {
        if (i.size() != 4)
            continue;
        for (int j = 0; j < 4; j++) {
            if (inter.find(i[j]) != inter.end())
                continue;
            inter[i[j]] = 'd';
            break;
        }
        break;
    }

    //6. The rest are G
    for (auto i : segment) {
        if (inter.find(i) != inter.end())
            continue;
        inter[i] = 'g';
        break;
    }

    //7. flip
    std::unordered_map<char, char> medi;
    for (auto& i : inter) {
        medi[i.second] = i.first;
    }

    //8. Generate result;
    std::unordered_map<std::string, long> result;
    result[generate_match("abcefg", medi)] = 0;
    result[generate_match("cf", medi)] = 1;
    result[generate_match("acdeg", medi)] = 2;
    result[generate_match("acdfg", medi)] = 3;
    result[generate_match("bcdf", medi)] = 4;
    result[generate_match("abdfg", medi)] = 5;
    result[generate_match("abdefg", medi)] = 6;
    result[generate_match("acf", medi)] = 7;
    result[generate_match("abcdefg", medi)] = 8;
    result[generate_match("abcdfg", medi)] = 9;
    return result;

}

long digits_string_to_long(std::vector<std::string>& str, std::unordered_map<std::string, long>& segment) {
    long output = 0;
    for (auto& i : str) {
        std::sort(i.begin(), i.end());
        output = output * 10 + segment[i];
    }
    return output;
}

long part_two(std::pair<std::vector<std::string>, std::vector<std::string>>& data) {
    long output = 0;
    for (long i = 0; i < data.first.size(); i++) {
        auto segment = parse_segment(data.first[i]);
        auto digits = split_data(data.second[i]);
        long nums = digits_string_to_long(digits, segment);
        output += nums;
    }
    return output;
}

int main()
{
    auto raw_test = read_data("example.txt");
    auto raw_input = read_data("day08.txt");
    std::cout << "Part 1 test result  : " << part_one(raw_test.second) << "\n";
    std::cout << "Part 1 input result : " << part_one(raw_input.second) << "\n";

    std::cout << "Part 2 test result  : " << part_two(raw_test) << "\n";
    std::cout << "Part 2 input result : " << part_two(raw_input) << "\n";
}