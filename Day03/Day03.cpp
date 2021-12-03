#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <bitset>

std::bitset<12> find_step_two(bool larger, const std::vector<std::bitset<12>>& vec) {
    std::vector<std::bitset<12>> current_list = vec;

    for (int i_inv = 0; i_inv < 12; i_inv++) {
        int i = 12 - 1 - i_inv;

        long count = 0;
        auto average = (float)current_list.size() / 2.f;
        for (auto& j : current_list) {
            if (j.test(i)) count++;
        }

        bool most = count >= average;
        if (!larger) most = !most;

        std::vector<std::bitset<12>> temp;
        for (auto& j : current_list) {
            if (j.test(i) == most)
                temp.push_back(j);
        }
        if (temp.size() == 0)
            continue;
        current_list.clear();
        current_list = temp;
        if (current_list.size() <= 1) break;
    }

    return current_list[0];
}

long convert_binary_string(const std::string& str) {
    long result = 0;
    for (int i = 0; i < 12; i++) {
        result = result << 1;
        result += str[i] - int('0');
    }
    return result;
}

int main()
{
    std::ifstream raw_data("day03.txt");
    //std::ifstream raw_data("example.txt");
    std::vector<std::bitset<12>>data_bitset;

    for (std::string i; std::getline(raw_data, i); ) {
        long j = convert_binary_string(i);
        data_bitset.push_back(j);
    }

    size_t max = data_bitset.size();
    std::vector<long> bit_count = { 0,0,0,0,0,0,0,0,0,0,0,0 };
    for (auto& i : data_bitset) {
        for (int j = 0; j < bit_count.size(); j++) {
            if (i.test(j)) bit_count[12 - 1 - j]++;
        }
    }

    long avg = data_bitset.size() / 2;
    long gamma = 0;
    long epsilon = 0;

    for (auto i : bit_count) {
        gamma = gamma << 1;
        epsilon = epsilon << 1;
        if (i > avg) {
            gamma++;
        } else {
            epsilon++;
        }
    }
    std::cout << "Gamma : " << gamma << " Epsilon : " << epsilon << "\n";
    std::cout << "Result for part 1 : " << gamma * epsilon << "\n";

    auto oksigen_set = find_step_two(true, data_bitset);
    long oksigen = oksigen_set.to_ulong();
    std::cout << "02  : " << oksigen_set << " " << oksigen << "\n";

    auto co2_set = find_step_two(false, data_bitset);
    long co2 = co2_set.to_ulong();
    std::cout << "CO2 : " << co2_set << " " << co2 << "\n";

    std::cout << "Result for part 2 : " << oksigen * co2 << "\n";
}