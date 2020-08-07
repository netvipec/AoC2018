#include <bits/stdc++.h>

typedef std::vector<char> vec_char;
typedef std::vector<std::string> vec_input;
typedef std::vector<std::pair<int, int>> adjacent_t;

vec_input read_input(std::string const& file_path) {
    vec_input input_values;

    std::ifstream infile(file_path);

    std::string line;
    while (std::getline(infile, line)) {
        input_values.push_back(line);

        // std::cout << line << std::endl;
    }

    return input_values;
}

void get_adjacent_cells_y(int x, int y, adjacent_t& adjacent, int size, bool original = false) {
    if (y > 0) {
        adjacent.emplace_back(x, y - 1);
    }
    if (y + 1 < size) {
        adjacent.emplace_back(x, y + 1);
    }
    if (!original) {
        adjacent.emplace_back(x, y);
    }
}

void get_adjacent_cells(int x, int y, adjacent_t& adjacent, int size) {
    adjacent.clear();
    adjacent.reserve(8);

    if (x > 0) {
        get_adjacent_cells_y(x - 1, y, adjacent, size);
    }
    if (x + 1 < size) {
        get_adjacent_cells_y(x + 1, y, adjacent, size);
    }
    get_adjacent_cells_y(x, y, adjacent, size, true);
}

void print_adjacent(int x, int y, adjacent_t const& adjacent) {
    std::cout << "(" << x << "," << y << ") -> ";
    std::for_each(std::cbegin(adjacent), std::cend(adjacent), [](auto const& n){
        std::cout << "(" << n.first << "," << n.second << "), ";
    });
    std::cout << std::endl;
}

void step(vec_input const& map, vec_input& new_map) {
    adjacent_t adjacent;

    for (int x = 0, x_end = map.size(); x < x_end; ++x) {
        for (int y = 0, y_end = map[x].size(); y < y_end; ++y) {
            assert(x_end == y_end);

            get_adjacent_cells(x, y, adjacent, x_end);

            // print_adjacent(x, y, adjacent);

            auto const wooded = std::accumulate(std::cbegin(adjacent), std::cend(adjacent), 0, [&](auto const& base, auto const& adj){
                return base + (map[adj.first][adj.second] == '|' ? 1 : 0);
            });
            auto const lumberyard = std::accumulate(std::cbegin(adjacent), std::cend(adjacent), 0, [&](auto const& base, auto const& adj){
                return base + (map[adj.first][adj.second] == '#' ? 1 : 0);
            });
            if (map[x][y] == '.') {
                if (wooded >= 3) {
                    new_map[x][y] = '|';
                } else {
                    new_map[x][y] = '.';
                }
            } else if (map[x][y] == '|') {
                if (lumberyard >= 3) {
                    new_map[x][y] = '#';
                } else {
                    new_map[x][y] = '|';
                }
            } else if (map[x][y] == '#') {
                if (lumberyard >= 1 && wooded >= 1) {
                    new_map[x][y] = '#';
                } else {
                    new_map[x][y] = '.';
                }
            }
        }
    }
}

void print_map(vec_input const& map) {
    std::for_each(std::cbegin(map), std::cend(map), [](auto const& row){
        std::cout << row << std::endl;
    });
    std::cout << std::endl;
}

void part1(vec_input const& input_values) {
    auto map = input_values;
    auto new_map = input_values;

    // print_map(map);

    constexpr int minutes = 10;
    for (int i = 0; i < minutes; ++i) {
        step(map, new_map);
        std::swap(map, new_map);

        // print_map(map);
    }

    auto const wooded = std::accumulate(std::cbegin(map), std::cend(map), 0, [](auto const& base, auto const& row){
        return base + std::accumulate(std::cbegin(row), std::cend(row), 0, [](auto const& base, auto const& cell){
            return base + (cell == '|' ? 1 : 0);
        });
    });

    auto const lumberyard = std::accumulate(std::cbegin(map), std::cend(map), 0, [](auto const& base, auto const& row){
        return base + std::accumulate(std::cbegin(row), std::cend(row), 0, [](auto const& base, auto const& cell){
            return base + (cell == '#' ? 1 : 0);
        });
    });

    std::cout << "Wooded: " << wooded << std::endl;
    std::cout << "Lumberyard: " << lumberyard << std::endl;
    std::cout << "Part1: " << wooded * lumberyard << std::endl;
}

void part2(vec_input const& input_values) {
    auto map = input_values;
    auto new_map = input_values;

    // std::cout << "0" << std::endl;
    // print_map(map);

    constexpr int64_t target = 1'000'000'000;

    constexpr int minutes = (target - 600) % 28;
    for (int i = 0; i < 600 + minutes; ++i) {
        step(map, new_map);
        std::swap(map, new_map);

        // std::cout << i + 1 << std::endl;
        // print_map(map);
    }

    auto const wooded = std::accumulate(std::cbegin(map), std::cend(map), 0, [](auto const& base, auto const& row){
        return base + std::accumulate(std::cbegin(row), std::cend(row), 0, [](auto const& base, auto const& cell){
            return base + (cell == '|' ? 1 : 0);
        });
    });

    auto const lumberyard = std::accumulate(std::cbegin(map), std::cend(map), 0, [](auto const& base, auto const& row){
        return base + std::accumulate(std::cbegin(row), std::cend(row), 0, [](auto const& base, auto const& cell){
            return base + (cell == '#' ? 1 : 0);
        });
    });

    std::cout << "Wooded: " << wooded << std::endl;
    std::cout << "Lumberyard: " << lumberyard << std::endl;
    std::cout << "Part2: " << wooded * lumberyard << std::endl;
}

int main() {
    auto const input_values = read_input("input");

    part1(input_values);
    part2(input_values);

    return 0;
}
