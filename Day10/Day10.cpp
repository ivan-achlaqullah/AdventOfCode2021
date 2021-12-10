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

enum class CHUNK_TYPE {
    CHUNK_OPEN,
    CHUNK_CLOSE
};

struct Closing
{
    char val;
    long err_val;
    CHUNK_TYPE type;
};

std::vector<std::string> load_input(std::string filename) {
    std::ifstream input(filename);
    std::vector<std::string> output;
    for (std::string i; std::getline(input, i);) {
        output.push_back(i);
    }
    return output;
}

long find_error_close(std::string& line, std::unordered_map<char, Closing> closeGroup) {
    std::string group_buff;
    for (auto& i : line) {
        if (closeGroup.find(i) == closeGroup.end()) {
            continue;
        }
        auto& expected = closeGroup[i];
        if (expected.type == CHUNK_TYPE::CHUNK_OPEN) {
            group_buff += i;
            continue;
        }
        if (group_buff[group_buff.size() - 1] != expected.val) {
            return expected.err_val;
        }
        else {
            group_buff.pop_back();
        }
    }
    return 0;
}

long long find_auto_complete(std::string& line, std::unordered_map<char, Closing> closeGroup) {
    std::string group_buff;
    for (auto& i : line) {
        if (closeGroup.find(i) == closeGroup.end()) {
            continue;
        }
        auto& expected = closeGroup[i];
        if (expected.type == CHUNK_TYPE::CHUNK_OPEN) {
            group_buff += i;
            continue;
        }
        if (group_buff[group_buff.size() - 1] != expected.val) {
            return 0;
        }
        else {
            group_buff.pop_back();
        }
    }
    if(group_buff.size() == 0)
        return 0;
    long long score = 0;
    for (long i = group_buff.size() - 1; i >= 0; i--) {
        score *= 5;
        score += closeGroup[group_buff[i]].err_val;
    }
    return score;
}

std::unordered_map<char, Closing> get_close_group() {
    std::unordered_map<char, Closing> closeGroup;
    closeGroup['('] = { ')', 1, CHUNK_TYPE::CHUNK_OPEN };
    closeGroup['['] = { ']', 2, CHUNK_TYPE::CHUNK_OPEN };
    closeGroup['{'] = { '}', 3, CHUNK_TYPE::CHUNK_OPEN };
    closeGroup['<'] = { '>', 4, CHUNK_TYPE::CHUNK_OPEN };

    closeGroup[')'] = { '(', 3, CHUNK_TYPE::CHUNK_CLOSE };
    closeGroup[']'] = { '[', 57, CHUNK_TYPE::CHUNK_CLOSE };
    closeGroup['}'] = { '{', 1197, CHUNK_TYPE::CHUNK_CLOSE };
    closeGroup['>'] = { '<', 25137, CHUNK_TYPE::CHUNK_CLOSE };
    return closeGroup;
}

long part_one(std::vector<std::string>& input) {
    auto group = get_close_group();
    long err_score = 0;
    for (auto& i : input) {
        err_score += find_error_close(i, group);
    }
    return err_score;
}

long long part_two(std::vector<std::string>& input) {
    auto group = get_close_group();
    std::vector<long long> score;
    for (auto& i : input) {
        long long temp = find_auto_complete(i, group);
        if (temp == 0)
            continue;
        score.push_back(temp);
    }
    std::sort(score.begin(), score.end());
    long middle = score.size() / 2;
    return score[middle];
}

int main()
{
    auto test = load_input("example.txt");
    auto input = load_input("day10.txt");

    std::cout << "Part 1 Test : " << part_one(test) << "\n";
    std::cout << "Part 1 : " << part_one(input) << "\n";

    std::cout << "Part 2 Test : " << part_two(test) << "\n";
    std::cout << "Part 2 : " << part_two(input) << "\n";
}