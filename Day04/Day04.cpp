#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>

struct membingo
{
    long number = 0;
    bool mark = false;
};

struct winbingo {
    bool bingo = false;
    long board = 0;
};

struct winresult
{
    long board = 0;
    long last_num = 0;
};

std::vector<long> read_drawn_num(std::ifstream& raw_data) {
    std::string str;
    std::getline(raw_data, str);
    std::stringstream str_stream(str);

    std::vector<long> result;
    for (std::string i; std::getline(str_stream, i, ','); ) {
        result.push_back(std::stol(i));
    }
    return result;
}

winbingo find_horizontal(std::vector<membingo>& board) {
    winbingo result;
    for (long i = 0; i < board.size(); i += 5) {
        result.bingo = board[i].mark &&
            board[i + 1].mark &&
            board[i + 2].mark &&
            board[i + 3].mark &&
            board[i + 4].mark;
        if (!result.bingo) continue;
        result.board = i / 25;
        std::cout << "Horizontal bingo found!\n";
        std::cout << "At board " << result.board + 1 << "\n";
        std::cout << board[i].number << "\n";
        std::cout << board[i + 1].number << "\n";
        std::cout << board[i + 2].number << "\n";
        std::cout << board[i + 3].number << "\n";
        std::cout << board[i + 4].number << "\n";
        return result;
    }
    return result;
}

winbingo find_vertical(std::vector<membingo>& board) {
    winbingo result;
    std::vector<long>order;
    long total_board = board.size() / 25;
    for (long i = 0; i < total_board; i++) {
        for (long j = 0; j < 5; j++) {
            order.push_back(i * 25 + j);
            order.push_back(i * 25 + j + 5);
            order.push_back(i * 25 + j + 10);
            order.push_back(i * 25 + j + 15);
            order.push_back(i * 25 + j + 20);
        }
    }

    for (long i = 0; i < order.size(); i += 5) {
        result.bingo = board[order[i]].mark &&
            board[order[i + 1]].mark &&
            board[order[i + 2]].mark &&
            board[order[i + 3]].mark &&
            board[order[i + 4]].mark;
        if (!result.bingo) continue;
        result.board = order[i] / 25;
        std::cout << "Vertical bingo found!\n";
        std::cout << "At board " << result.board + 1 << "\n";
        std::cout << board[order[i]].number << "\n";
        std::cout << board[order[i + 1]].number << "\n";
        std::cout << board[order[i + 2]].number << "\n";
        std::cout << board[order[i + 3]].number << "\n";
        std::cout << board[order[i + 4]].number << "\n";
        return result;
    }
    return result;
}

winresult winner_board(const std::vector<long>& drawn, std::vector<membingo>& board) {
    winresult result;
    for (long i = 0; i < drawn.size(); i++) {
        for (auto& j : board) {
            if (j.number == drawn[i]) j.mark = true;
        }
        if (i < 5) continue;
        auto win = find_horizontal(board);
        if (!win.bingo) win = find_vertical(board);
        if (win.bingo) {
            result.board = win.board;
            result.last_num = drawn[i];
            return result;
        }
    }
    return result;
}

int main()
{
    //std::ifstream raw_data("example.txt");
    std::ifstream raw_data("day04.txt");

    std::vector<long> drawn_num = read_drawn_num(raw_data);

    std::vector<membingo> bingo_board;
    std::regex find_board_number("(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)$");
    for (std::string i; std::getline(raw_data, i); ) {
        if (i.size() == 0) {
            continue;
        }
        std::smatch match;
        if (!std::regex_search(i, match, find_board_number)) continue;
        for (int j = 1; j < match.size(); j++) {
            membingo temp = { std::stol(match[j].str()), false };
            bingo_board.push_back(temp);
        }
    }

    auto bingo = winner_board(drawn_num, bingo_board);
    long total_unmarked = 0;
    for (long i = 0; i < 25; i++) {
        long j = bingo.board * 25 + i;
        if (bingo_board[j].mark) continue;
        total_unmarked += bingo_board[j].number;
    }
    std::cout << "Total Unmarked : " << total_unmarked << "\n";
    std::cout << "Last drawn number : " << bingo.last_num << "\n";
    std::cout << "Result for Part 1 : " << total_unmarked * bingo.last_num << "\n";
}
