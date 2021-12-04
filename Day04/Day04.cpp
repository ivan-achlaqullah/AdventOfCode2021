#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <unordered_set>

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

void find_horizontal(std::vector<membingo>& board, std::vector<winbingo>& win_list) {
    for (long i = 0; i < board.size(); i += 5) {
        winbingo result;
        result.bingo = board[i].mark &&
            board[i + 1].mark &&
            board[i + 2].mark &&
            board[i + 3].mark &&
            board[i + 4].mark;
        if (!result.bingo) continue;
        result.board = i / 25;
        win_list.push_back(result);
    }
}

std::vector<long> vertical_order(long size) {
    long total_board = size / 25;
    std::vector<long> order;
    for (long i = 0; i < total_board; i++) {
        for (long j = 0; j < 5; j++) {
            order.push_back(i * 25 + j);
            order.push_back(i * 25 + j + 5);
            order.push_back(i * 25 + j + 10);
            order.push_back(i * 25 + j + 15);
            order.push_back(i * 25 + j + 20);
        }
    }
    return order;
}

winbingo find_vertical(std::vector<membingo>& board) {
    winbingo result;
    std::vector<long> order = vertical_order(board.size());

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

void find_vertical(std::vector<membingo>& board, std::vector<winbingo>& win_list) {
    std::vector<long> order = vertical_order(board.size());
    for (long i = 0; i < order.size(); i += 5) {
        winbingo result;
        result.bingo = board[order[i]].mark &&
            board[order[i + 1]].mark &&
            board[order[i + 2]].mark &&
            board[order[i + 3]].mark &&
            board[order[i + 4]].mark;
        if (!result.bingo) continue;
        result.board = order[i] / 25;
        win_list.push_back(result);
    }
}

winresult winner_board(const std::vector<long>& drawn, std::vector<membingo>& board) {
    winresult result;
    for (long i = 0; i < drawn.size(); i++) {
        for (auto& j : board) {
            if (j.number == drawn[i]) j.mark = true;
        }
        if (i < 4) continue;
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

winresult last_board(const std::vector<long>& drawn, std::vector<membingo>& board) {
    winresult last_result;
    std::unordered_set<long> past_winner;
    for (long i = 0; i < drawn.size(); i++) {
        for (auto& j : board) {
            if (j.number == drawn[i]) j.mark = true;
        }
        if (i < 4) continue;
        std::vector<winbingo> win_list;
        find_horizontal(board, win_list);
        find_vertical(board, win_list);
        for (auto& j : win_list) {
            if (j.bingo && past_winner.find(j.board) == past_winner.end()) {
                winresult temp;
                temp.board = j.board;
                temp.last_num = drawn[i];
                past_winner.insert(j.board);
                last_result = temp;
            }
        }
    }
    return last_result;
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
    std::cout << "Result for Part 1 : " << total_unmarked * bingo.last_num << "\n\n";

    // Clear all mark for Part 2
    for (auto& i : bingo_board) {
        i.mark = false;
    }

    auto last = last_board(drawn_num, bingo_board);
    std::cout << "Last winner found at board " << last.board + 1 << "\n";
    
    // Clear all mark again, then mark it again until it reach last_num
    total_unmarked = 0;
    for (auto& i : bingo_board) {
        i.mark = false;
    }
    for (auto i : drawn_num) {
        for (auto& j : bingo_board) {
            if (j.number == i) j.mark = true;
        }
        if (i == last.last_num) break;
    }
    
    for (long i = 0; i < 25; i++) {
        long j = last.board * 25 + i;
        if (bingo_board[j].mark) continue;
        total_unmarked += bingo_board[j].number;
    }
    std::cout << "Total Unmarked : " << total_unmarked << "\n";
    std::cout << "Last drawn number : " << last.last_num << "\n";
    std::cout << "Result for Part 1 : " << total_unmarked * last.last_num << "\n\n";
}
