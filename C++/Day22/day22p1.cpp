#include <bits/stdc++.h>

int64_t target_x = 0;
int64_t target_y = 0;
int64_t depth = 0;
typedef std::pair<int64_t, int64_t> pos_t;
typedef std::pair<int64_t, pos_t> vec_input;

constexpr std::array<pos_t, 4> D = { pos_t{ 0, -1 }, pos_t{ 0, 1 }, pos_t{ -1, 0 }, pos_t{ 1, 0 } };

struct cell {
    std::pair<int64_t, int64_t> pos;
    int tool_type;

    cell(std::pair<int64_t, int64_t> p, int tt) : pos(p), tool_type(tt) {}

    bool operator==(cell const& o) const { return std::tie(pos, tool_type) == std::tie(o.pos, o.tool_type); }
    bool operator<(cell const& o) const { return std::tie(pos, tool_type) < std::tie(o.pos, o.tool_type); }
    friend std::ostream& operator<<(std::ostream& os, cell const& c);
};

std::ostream& operator<<(std::ostream& os, cell const& c) {
    os << "[(" << c.pos.first << "," << c.pos.second << ")," << c.tool_type << "]";
    return os;
}

struct cell_hash {
    size_t operator()(const cell& c) const {
        auto hash1 = std::hash<int64_t>{}(c.pos.first);
        auto hash2 = std::hash<int64_t>{}(c.pos.second);
        auto hash3 = std::hash<int>{}(c.tool_type);
        return hash1 ^ hash2 ^ hash3;
    }
};

std::map<std::pair<int64_t, int64_t>, int64_t> map;

// trim from start (in place)
static inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
                return !std::isspace(ch);
            }));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(),
                         s.rend(),
                         [](int ch) {
                             return !std::isspace(ch);
                         })
                .base(),
            s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}

static std::vector<std::string> split(std::string const& s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vec_input read_input(std::string const& file_path) {
    vec_input input_values;

    std::ifstream infile(file_path);

    std::string depth;
    std::string target;

    std::getline(infile, depth);
    auto const depth_part = split(depth, ' ');
    assert(depth_part.size() == 2);
    input_values.first = std::stoi(depth_part[1]);
    std::cout << "depth: " << input_values.first << std::endl;

    std::getline(infile, target);
    auto const target_part = split(target, ' ');
    assert(target_part.size() == 2);
    auto const target_pos_part = split(target_part[1], ',');
    assert(target_pos_part.size() == 2);
    input_values.second.first = std::stoi(target_pos_part[0]);
    input_values.second.second = std::stoi(target_pos_part[1]);
    std::cout << "target: " << input_values.second.first << "," << input_values.second.second << std::endl;

    return input_values;
}

int64_t erosion_level(int64_t x, int64_t y);

int64_t geologic_index(int64_t x, int64_t y) {
    if ((x == 0 && y == 0) || (x == target_x && y == target_y)) {
        return 0;
    }

    if (y == 0) {
        return x * 16807;
    }
    if (x == 0) {
        return y * 48271;
    }

    auto const key1 = std::make_pair(x, y - 1);
    auto it1 = map.find(key1);
    if (it1 == std::cend(map)) {
        it1 = map.emplace(key1, erosion_level(x, y - 1)).first;
    }
    auto const key2 = std::make_pair(x - 1, y);
    auto it2 = map.find(key2);
    if (it2 == std::cend(map)) {
        it2 = map.emplace(key2, erosion_level(x - 1, y)).first;
    }

    return it1->second * it2->second;
}

int64_t erosion_level(int64_t x, int64_t y) {
    auto const gi = geologic_index(x, y);
    return (gi + depth) % 20183;
}

int64_t erosion_level(std::pair<int64_t, int64_t> const& p) { return erosion_level(p.first, p.second); }

int region_type(int64_t erosion_level) { return erosion_level % 3; }

void part1(vec_input const&) {
    erosion_level(target_x, target_y);

    int64_t risk = 0;
    for (int y = 0, y_end = target_y; y <= y_end; ++y) {
        for (int x = 0, x_end = target_x; x <= x_end; ++x) {
            risk += region_type(erosion_level(x, y));
        }
    }

    std::cout << "Part1: " << risk << std::endl;
}

void check_path(std::vector<std::pair<cell, int64_t>> const& path) {
    int pos = 0;
    std::for_each(std::cbegin(path) + 1, std::cend(path), [&](auto const& p) {
        auto const new_el = erosion_level(p.first.pos);
        auto const new_rt = region_type(new_el);
        auto const old_el = erosion_level(path[pos].first.pos);
        auto const old_rt = region_type(old_el);

        switch (new_rt) {
            case 0:
                if (p.first.tool_type == 2)
                    std::cout << "Error found: old_rt: " << old_rt << ", new_rt: " << new_rt << " " << path[pos].first
                              << " -> " << p.first << std::endl;
                break;
            case 1:
                if (p.first.tool_type == 1)
                    std::cout << "Error found: old_rt: " << old_rt << ", new_rt: " << new_rt << " " << path[pos].first
                              << " -> " << p.first << std::endl;
                break;
            case 2:
                if (p.first.tool_type == 0)
                    std::cout << "Error found: old_rt: " << old_rt << ", new_rt: " << new_rt << " " << path[pos].first
                              << " -> " << p.first << std::endl;
                break;
        }
        switch (old_rt) {
            case 0:
                if (p.first.tool_type == 2)
                    std::cout << "Error found: old_rt: " << old_rt << ", new_rt: " << new_rt << " " << path[pos].first
                              << " -> " << p.first << std::endl;
                break;
            case 1:
                if (p.first.tool_type == 1)
                    std::cout << "Error found: old_rt: " << old_rt << ", new_rt: " << new_rt << " " << path[pos].first
                              << " -> " << p.first << std::endl;
                break;
            case 2:
                if (p.first.tool_type == 0)
                    std::cout << "Error found: old_rt: " << old_rt << ", new_rt: " << new_rt << " " << path[pos].first
                              << " -> " << p.first << std::endl;
                break;
        }

        if (path[pos].first.tool_type != p.first.tool_type && path[pos].second + 8 != p.second) {
            std::cout << "Error found: old_rt: " << old_rt << ", new_rt: " << new_rt << " " << path[pos].first << " -> "
                      << p.first << std::endl;
        }

        ++pos;
    });
}

bool valid_move(int old_rt, int new_rt, int tool_type) {
    switch (old_rt) {
        case 0:
            if (tool_type == 2)
                return false;
            break;
        case 1:
            if (tool_type == 1)
                return false;
            break;
        case 2:
            if (tool_type == 0)
                return false;
            break;
    }
    switch (new_rt) {
        case 0:
            if (tool_type == 2)
                return false;
            break;
        case 1:
            if (tool_type == 1)
                return false;
            break;
        case 2:
            if (tool_type == 0)
                return false;
            break;
    }
    return true;
}

template <typename It, typename Visited>
void print_path(It end_path, Visited const& visited) {
    std::vector<std::pair<cell, int64_t>> path;

    path.emplace_back(end_path->first, end_path->second);

    auto it = visited.find(end_path->first);
    while (it->first.pos.first != 0 || it->first.pos.second != 0) {
        std::for_each(std::cbegin(D), std::cend(D), [&](auto const& d) {
            auto const& p = it->first;
            if (p.pos.first + d.first < 0 || p.pos.second + d.second < 0)
                return;

            auto const is_target = p.pos.first == target_x && p.pos.second == target_y;

            auto const new_el = erosion_level(p.pos);
            auto const new_rt = region_type(new_el);

            auto const np = std::make_pair(p.pos.first + d.first, p.pos.second + d.second);
            auto const old_el = erosion_level(np);
            auto const old_rt = region_type(old_el);
            auto d_it = visited.lower_bound({ np, 0 });

            while (d_it != std::cend(visited) && d_it->first.pos.first == np.first
                   && d_it->first.pos.second == np.second) {
                auto const move_cost = 1 + (p.tool_type != d_it->first.tool_type ? 7 : 0) + ((is_target && (p.tool_type != 1)) ? 7 : 0);
                if (valid_move(old_rt, new_rt, p.tool_type) && d_it->second + move_cost == it->second) {
                    path.push_back(*d_it);
                    it = d_it;
                    return;
                }
                ++d_it;
            }
        });
    }

    std::reverse(std::begin(path), std::end(path));
    check_path(path);

    std::cout << "Path:" << std::endl;
    std::for_each(std::cbegin(path), std::cend(path), [](auto const& p) {
        std::cout << p.first << " = " << p.second << std::endl;
    });
}

void part2(vec_input const&) {
    std::map<cell, int64_t> visited;

    std::vector<std::pair<cell, int64_t>> positions{ { cell{ { 0, 0 }, 1 }, 0 } };
    std::vector<std::pair<cell, int64_t>> new_positions;

    std::vector<std::pair<cell, int64_t>> targets;

    visited.emplace(positions.front().first, 0);

    // rocky as ., wet as =, narrow as |
    for (; positions.size();) {
        std::for_each(std::cbegin(positions), std::cend(positions), [&](auto const& pos) {
            std::for_each(std::cbegin(D), std::cend(D), [&](auto const& d) {
                auto const& p = pos.first;
                auto const& p_best = pos.second;
                if (p.pos.first + d.first < 0 || p.pos.second + d.second < 0)
                    return;

                auto const np = std::make_pair(p.pos.first + d.first, p.pos.second + d.second);
                auto const is_target = np.first == target_x && np.second == target_y;
                auto const new_el = erosion_level(np);
                auto const new_rt = region_type(new_el);
                auto const old_el = erosion_level(p.pos);
                auto const old_rt = region_type(old_el);

                for (int tt = 0; tt < 3; ++tt) {
                    if (valid_move(old_rt, new_rt, tt)) {
                        auto it_tt = visited.find(cell{ np, tt });
                        auto const move_cost = 1 + (p.tool_type != tt ? 7 : 0) + ((is_target && (tt != 1)) ? 7 : 0);
                        if (it_tt == std::cend(visited) || it_tt->second > p_best + move_cost) {
                            new_positions.emplace_back(cell{ np, tt }, p_best + move_cost);
                            visited[cell{ np, tt }] = p_best + move_cost;
                        }
                    }
                }
            });
        });

        targets.clear();
        std::copy_if(std::cbegin(visited), std::cend(visited), std::back_inserter(targets), [](auto const& v) {
            return v.first.pos.first == target_x && v.first.pos.second == target_y;
        });

        if (targets.size() > 0) {
            auto const best_target_it =
                std::min_element(std::cbegin(targets), std::cend(targets), [](auto const& lhs, auto const& rhs) {
                    return lhs.second < rhs.second;
                });
            std::cout << "Best target until now: " << best_target_it->first << " = " << best_target_it->second
                      << std::endl;
            auto it = std::remove_if(std::begin(new_positions), std::end(new_positions), [&](auto& np) {
                auto const md = std::abs(np.first.pos.first - target_x) + std::abs(np.first.pos.second - target_y);
                return np.second + md >= best_target_it->second
                       || (np.first.pos.first == target_x && np.first.pos.second == target_y);
            });
            new_positions.erase(it, std::end(new_positions));
        }

        std::sort(std::begin(new_positions), std::end(new_positions));
        auto it = std::unique(std::begin(new_positions), std::end(new_positions), [](auto const& lhs, auto const& rhs) {
            return lhs.first == rhs.first;
        });
        new_positions.erase(it, std::end(new_positions));

        std::swap(positions, new_positions);
        new_positions.clear();

        // std::for_each(std::cbegin(positions), std::cend(positions), [](auto const& p) {
        //     std::cout << p.first << " = " << p.second << std::endl;
        // });
        // std::cout << std::endl;
    }

    targets.clear();
    std::copy_if(std::cbegin(visited), std::cend(visited), std::back_inserter(targets), [](auto const& v) {
        return v.first.pos.first == target_x && v.first.pos.second == target_y;
    });

    assert(targets.size() > 0);
    auto const best_target_it =
        std::min_element(std::cbegin(targets), std::cend(targets), [](auto const& lhs, auto const& rhs) {
            return lhs.second < rhs.second;
        });

    // print_path(best_target_it, visited);

    std::cout << "Part2: " << best_target_it->second << std::endl;
}

void print_map() {
    for (int y = 0; y <= target_y + 5; ++y) {
        for (int x = 0; x <= target_x + 5; ++x) {
            auto const el = erosion_level(x, y);
            auto const rt = region_type(el);

            if (x == 0 && y == 0) {
                std::cout << "M";
                continue;
            }
            if (x == target_x && y == target_y) {
                std::cout << "T";
                continue;
            }

            switch (rt) {
                case 0:
                    std::cout << ".";
                    break;
                case 1:
                    std::cout << "=";
                    break;
                case 2:
                    std::cout << "|";
                    break;
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    auto const input_values = read_input("input");

    depth = input_values.first;
    target_x = input_values.second.first;
    target_y = input_values.second.second;
    // depth = 510;
    // target_x = 10;
    // target_y = 10;

    part1(input_values);
    // print_map();
    part2(input_values);

    return 0;
}