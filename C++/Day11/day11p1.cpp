#include <bits/stdc++.h>

typedef std::vector<int64_t> vec_int;
typedef int64_t vec_input;

vec_input read_input(std::string const& ) {
    return 7400;
}

int64_t power_level(int64_t x, int64_t y, int64_t serial_number) {
    auto const rack_id = x + 10;
    auto const start_power_level = rack_id * y;
    auto const added = start_power_level + serial_number;
    auto const mult = added * rack_id;
    auto const cent = (mult / 100) % 10;
    return cent - 5;
}

void part1(vec_input const& input_values) {
    auto const serial_number = input_values;

    std::vector<std::vector<int64_t>> power_map(300, std::vector<int64_t>(300));
    for (int x = 1; x <= 300; ++x) {
        for (int y = 1; y <= 300; ++y) {
            power_map[x-1][y-1] = power_level(x, y, serial_number);
        }
    }

    std::vector<std::vector<int64_t>> power_level_map(297, std::vector<int64_t>(297));
    for (int x = 0; x < 300 - 3; ++x) {
        for (int y = 0; y < 300 - 3; ++y) {
            for (int dx = 0; dx < 3; ++dx) {
                for (int dy = 0; dy < 3; ++dy) {
                    power_level_map[x][y] += power_map[x + dx][y + dy];
                }
            }
        }
    }

    int64_t best_power_level = std::numeric_limits<int64_t>::min();
    int64_t best_x = -1;
    int64_t best_y = -1;

    for (int x = 0; x < 300 - 3; ++x) {
        auto max_it = std::max_element(std::cbegin(power_level_map[x]), std::cend(power_level_map[x]));
        if (*max_it > best_power_level) {
            best_power_level = *max_it;
            best_x = x + 1;
            best_y = std::distance(std::cbegin(power_level_map[x]), max_it) + 1;
        }
    }

    std::cout << "Part1: " << best_x << "," << best_y << " -> " << best_power_level << std::endl;
}

void part2(vec_input const& input_values) {
    auto const serial_number = input_values;

    std::vector<std::vector<int64_t>> power_map(300, std::vector<int64_t>(300));
    for (int x = 1; x <= 300; ++x) {
        for (int y = 1; y <= 300; ++y) {
            power_map[x-1][y-1] = power_level(x, y, serial_number);
        }
    }

    int64_t best_power_level = std::numeric_limits<int64_t>::min();
    int64_t best_x = -1;
    int64_t best_y = -1;
    int64_t best_size = -1;

    std::vector<std::vector<int64_t>> power_level_map(300, std::vector<int64_t>(300));
    for (int x = 0; x < 300 - 2; ++x) {
        for (int y = 0; y < 300 - 2; ++y) {
            for (int dx = 0; dx < 2; ++dx) {
                for (int dy = 0; dy < 2; ++dy) {
                    power_level_map[x][y] += power_map[x + dx][y + dy];
                }
            }
        }
    }

    for (int max_d = 3; max_d <= 300; ++max_d) {
        auto const final_index = 300 - max_d;

        for (int x = 0; x < final_index; ++x) {
            for (int y = 0; y < final_index; ++y) {
                power_level_map[x][y] += std::accumulate(std::cbegin(power_map[x + max_d - 1]) + y, std::cbegin(power_map[x + max_d - 1]) + y + max_d, 0);
                for (int dx = 0; dx < max_d - 1; ++dx) {
                    power_level_map[x][y] += power_map[x + dx][y + max_d - 1];
                }
            }
        }

        for (int x = 0; x < final_index; ++x) {
            auto max_it = std::max_element(std::cbegin(power_level_map[x]), std::cbegin(power_level_map[x]) + final_index);
            if (*max_it > best_power_level) {
                best_power_level = *max_it;
                best_x = x + 1;
                best_y = std::distance(std::cbegin(power_level_map[x]), max_it) + 1;
                best_size = max_d;
            }
        }
    }

    std::cout << "Part1: " << best_x << "," << best_y << "," << best_size << " -> " << best_power_level << std::endl;
}

int main() {
    auto const input_values = read_input("input");
    
    part1(input_values);
    part2(input_values);

    return 0;
}