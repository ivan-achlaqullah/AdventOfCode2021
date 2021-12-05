#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <unordered_map>
#include <algorithm>


struct Vertex {
    long x = 0;
    long y = 0;

public:
    std::string to_string() {
        std::string temp;
        temp += "X" + std::to_string(x) + "Y" + std::to_string(y);
        return temp;
    }
};

struct Line {
    Vertex first;
    Vertex last;
};

void sort_swap(long& x, long& y) {
    if (x < y)
        return;

    long temp = x;
    x = y;
    y = temp;
}

void draw_line_vertical(const Line& line, std::unordered_map<std::string, long>& texture) {
    long x = line.first.x;
    long y_min = line.first.y;
    long y_max = line.last.y;
    
    sort_swap(y_min, y_max);

    for (long i = y_min; i <= y_max; i++) {
        Vertex temp = { x, i };
        if (texture.find(temp.to_string()) != texture.end()) {
            texture[temp.to_string()]++;
        }
        else {
            texture[temp.to_string()] = 1;
        }
    }
}

void draw_line_horzontal(const Line& line, std::unordered_map<std::string, long>& texture) {
    long y = line.first.y;
    long x_min = line.first.x;
    long x_max = line.last.x;

    sort_swap (x_min, x_max);

    for (long i = x_min; i <= x_max; i++) {
        Vertex temp = { i, y };
        if (texture.find(temp.to_string()) != texture.end()) {
            texture[temp.to_string()]++;
        }
        else {
            texture[temp.to_string()] = 1;
        }
    }
}

bool is_diagonal(const Line& line) {
    long x = line.first.x - line.last.x;
    long y = line.first.y - line.last.y;
    return x % y == 0;
}

void draw_line_diagonal(const Line& line, std::unordered_map<std::string, long>& texture) {
    Vertex offset;
    long step = 0;
    bool positive = false;
    if (line.first.x < line.last.x) {
        offset = line.first;
        positive = line.first.y < line.last.y;
        step = line.last.x - line.first.x;
    }
    else {
        offset = line.last;
        positive = line.last.y < line.first.y;
        step = line.first.x - line.last.x;
    }

    for (long i = 0; i <= step; i++) {
        Vertex temp = offset;
        temp.x += i;
        if (positive) {
            temp.y += i;
        }
        else {
            temp.y -= i;
        }

        if (texture.find(temp.to_string()) != texture.end()) {
            texture[temp.to_string()]++;
        }
        else {
            texture[temp.to_string()] = 1;
        }
    }
}

void draw_line(const Line& line, std::unordered_map<std::string, long>& texture, bool diagonal) {
    if (line.first.x == line.last.x) {
        draw_line_vertical(line, texture);
    }
    else if (line.first.y == line.last.y) {
        draw_line_horzontal(line, texture);
    }
    else if (is_diagonal(line) && diagonal) {
        draw_line_diagonal(line, texture);
    }
}

void render_texture(std::unordered_map<std::string, long>& texture) {
    for (long y = 0; y <= 9; y++) {
        for (long x = 0; x <= 9; x++) {
            Vertex temp = { x, y };
            if (texture.find(temp.to_string()) == texture.end()) {
                std::cout << ".";
                continue;
            }
            std::cout << texture[temp.to_string()];
        }
        std::cout << "\n";
    }
}

int main()
{
    std::vector<Line> line_list;
    std::regex find_pattern("(\\d+).(\\d+) .. (\\d+).(\\d+)");

    //std::ifstream raw_data("example.txt");
    std::ifstream raw_data("day05.txt");
    for (std::string i; std::getline(raw_data, i); ) {
        std::smatch match;
        if (!std::regex_search(i, match, find_pattern)) continue;
        Line temp;
        temp.first.x = std::stol(match[1].str());
        temp.first.y = std::stol(match[2].str());
        temp.last.x = std::stol(match[3].str());
        temp.last.y = std::stol(match[4].str());
        line_list.push_back(temp);
    }

    std::unordered_map<std::string, long> render;
    for (auto& i : line_list) {
        draw_line(i, render, false);
    }

    long overlap = 0;
    for (auto& i : render) {
        if (i.second >= 2) overlap++;
    }

    std::cout << "Result for Part 1 : " << overlap << "\n";
    //render_texture(render);

    std::unordered_map<std::string, long> render2;
    for (auto& i : line_list) {
        draw_line(i, render2, true);
    }

    overlap = 0;
    for (auto& i : render2) {
        if (i.second >= 2) overlap++;
    }

    std::cout << "Result for Part 2 : " << overlap << "\n";
    //render_texture(render2);
}