#include <bits/stdc++.h>

struct cell {
    int x;
    int y;
    char t;

    cell(int xx, int yy, char tt)
        : x(xx), y(yy), t(tt) {}

    bool operator<(cell const& other) const {
        return std::tie(y, x, t) < std::tie(other.y, other.x, other.t);
    }

    friend std::ostream& operator<<(std::ostream& os, cell const& c);
};

std::ostream& operator<<(std::ostream& os, cell const& c) {
    os << "(" << c.x << "," << c.y << ",'" << c.t << "')";
    return os;
}

typedef std::vector<int> vec_int;
typedef std::pair<int, int> pos_t;
typedef std::vector<pos_t> positions_t;
typedef std::vector<positions_t> backtrack_t;
typedef std::string vec_input;
typedef std::vector<std::string> map_t;

vec_input read_input(std::string const& file_path) {
    vec_input input_values;

    std::ifstream infile(file_path);
    std::getline(infile, input_values);
    
    return input_values;
}

void print_map(map_t const& map) {
    std::for_each(std::cbegin(map), std::cend(map), [](auto const& row){
        std::cout << row << std::endl;
    });
    std::cout << std::endl;
}

map_t generate_map(std::string const& regex, pos_t& origin) {
    std::set<cell> mapc;

    backtrack_t backtrack;
    positions_t actual_pos{{0, 0}};
    std::for_each(std::cbegin(regex), std::cend(regex), [&](auto const& c){
        if (c == '^' || c == '$')
            return;
        if (c == '(') {
            // Save actual_pos as the beginning of all the diverse path.
            // Save actual_pos as the 
            backtrack.push_back(actual_pos);
            backtrack.push_back(positions_t{});

            return;
        }
        if (c == '|') {
            backtrack.back().insert(std::end(backtrack.back()), std::cbegin(actual_pos), std::cend(actual_pos));
            std::sort(std::begin(backtrack.back()), std::end(backtrack.back()));
            auto const last = std::unique(std::begin(backtrack.back()), std::end(backtrack.back()));
            backtrack.back().erase(last, std::end(backtrack.back()));
            actual_pos = backtrack[backtrack.size() - 2];

            return;
        }
        if (c == ')') {
            actual_pos = backtrack.back();
            backtrack.pop_back();
            backtrack.pop_back();

            return;
        }

        // Update actual_pos
        switch (c) {
            case 'N':
                std::for_each(std::begin(actual_pos), std::end(actual_pos), [&](auto& pos){
                    mapc.insert(cell(pos.first, pos.second - 1, '-'));
                    mapc.insert(cell(pos.first, pos.second - 2, '.'));
                    pos.second -= 2;
                });
                break;
            case 'S':
                std::for_each(std::begin(actual_pos), std::end(actual_pos), [&](auto& pos){
                    mapc.insert(cell(pos.first, pos.second + 1, '-'));
                    mapc.insert(cell(pos.first, pos.second + 2, '.'));
                    pos.second += 2;
                });
                break;
            case 'W':
                std::for_each(std::begin(actual_pos), std::end(actual_pos), [&](auto& pos){
                    mapc.insert(cell(pos.first - 1, pos.second, '|'));
                    mapc.insert(cell(pos.first - 2, pos.second, '.'));
                    pos.first -= 2;
                });
                break;
            case 'E':
                std::for_each(std::begin(actual_pos), std::end(actual_pos), [&](auto& pos){
                    mapc.insert(cell(pos.first + 1, pos.second, '|'));
                    mapc.insert(cell(pos.first + 2, pos.second, '.'));
                    pos.first += 2;
                });
                break;
        }
    });

    auto const& minmax_y = std::minmax_element(std::cbegin(mapc), std::cend(mapc), [](auto const& lhs, auto const& rhs){
        return lhs.y < rhs.y;
    });
    auto const& minmax_x = std::minmax_element(std::cbegin(mapc), std::cend(mapc), [](auto const& lhs, auto const& rhs){
        return lhs.x < rhs.x;
    });

    map_t map(minmax_y.second->y - minmax_y.first->y + 3, std::string(minmax_x.second->x - minmax_x.first->x + 3, '#'));
    std::for_each(std::cbegin(mapc), std::cend(mapc), [&](auto const& cell){
        map[cell.y - minmax_y.first->y + 1][cell.x - minmax_x.first->x + 1] = cell.t;
    });

    origin.second = 1 - minmax_y.first->y;
    origin.first = 1 - minmax_x.first->x;
    map[origin.second][origin.first] = 'X';

    return map;
}

void part1(vec_input const& input_values) {
    pos_t origin;
    auto map = generate_map(input_values, origin);
    // print_map(map);

    std::set<pos_t> visited{origin};
    positions_t possitions {origin};
    positions_t new_possitions;

    constexpr std::array<pos_t, 4> D = {pos_t{0, -1}, pos_t{0, 1}, pos_t{-1, 0}, pos_t{1, 0}};

    int counter = -1;
    while (possitions.size() > 0) {
        ++counter;
        std::for_each(std::cbegin(possitions), std::cend(possitions), [&](auto const& pos){
            std::for_each(std::cbegin(D), std::cend(D), [&](auto const& d){
                if (map[pos.second + d.second][pos.first + d.first] == '-'
                 || map[pos.second + d.second][pos.first + d.first] == '|') {
                    pos_t const new_pos{pos.first + 2 * d.first, pos.second + 2 * d.second};
                    auto const found_it = visited.find(new_pos);
                    if (found_it == std::cend(visited)) {
                        new_possitions.push_back(new_pos);
                        visited.insert(new_pos);
                    }
                }
            });
        });

        std::swap(possitions, new_possitions);
        new_possitions.clear();
    }

    std::cout << "Part1: " << counter << std::endl;
}

void part2(vec_input const& input_values) {
    pos_t origin;
    auto map = generate_map(input_values, origin);

    std::set<pos_t> visited{origin};
    positions_t possitions {origin};
    positions_t new_possitions;

    constexpr std::array<pos_t, 4> D = {pos_t{0, -1}, pos_t{0, 1}, pos_t{-1, 0}, pos_t{1, 0}};

    int saved_rooms = 0;
    int counter = 0;
    while (possitions.size() > 0) {
        ++counter;
        std::for_each(std::cbegin(possitions), std::cend(possitions), [&](auto const& pos){
            std::for_each(std::cbegin(D), std::cend(D), [&](auto const& d){
                if (map[pos.second + d.second][pos.first + d.first] == '-'
                 || map[pos.second + d.second][pos.first + d.first] == '|') {
                    pos_t const new_pos{pos.first + 2 * d.first, pos.second + 2 * d.second};
                    auto const found_it = visited.find(new_pos);
                    if (found_it == std::cend(visited)) {
                        new_possitions.push_back(new_pos);
                        visited.insert(new_pos);
                    }
                }
            });
        });

        if (counter >= 1000) {
            saved_rooms += new_possitions.size();
        }

        std::swap(possitions, new_possitions);
        new_possitions.clear();
    }

    auto const row_rooms = (map.size() - 1) / 2;
    std::cout << "Total rooms: " << row_rooms * row_rooms << std::endl;
    std::cout << "Part2: " << saved_rooms << std::endl;
}

int main() {
    auto const input_values = read_input("input");
    // std::cout << input_values << std::endl;

    part1(input_values);
    part2(input_values);

    return 0;
}