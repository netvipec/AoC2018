#include <bits/stdc++.h>

enum class Direction { Down, Up };

struct pos {
    int x;
    int y;
    Direction d;

    pos(int xx, int yy, Direction dd) : x(xx), y(yy), d(dd) {}

    bool operator==(pos const& other) const { return std::tie(y, x, d) == std::tie(other.y, other.x, other.d); }
    bool operator<(pos const& other) const { return std::tie(y, x, d) < std::tie(other.y, other.x, other.d); }

    friend std::ostream& operator<<(std::ostream& os, pos const& p);
};

std::ostream& operator<<(std::ostream& os, pos const& p) {
    os << p.x << "," << p.y << "," << (p.d == Direction::Down ? "Down" : "Up");
    return os;
}

struct line {
    char hv;
    int hv_num;

    char hv_range;
    int hv_start;
    int hv_end;

    line(char hv, int hv_num, char hv_range, int hv_start, int hv_end)
        : hv(hv), hv_num(hv_num), hv_range(hv_range), hv_start(hv_start), hv_end(hv_end) {}

    friend std::ostream& operator<<(std::ostream& os, line const& l);
};

std::ostream& operator<<(std::ostream& os, line const& l) {
    os << l.hv << "=" << l.hv_num << ", " << l.hv_range << "=" << l.hv_start << ".." << l.hv_end;
    return os;
}

typedef std::vector<char> vec_char;
typedef std::vector<vec_char> vec_vec_char;
typedef std::vector<line> vec_input;

// trim from start (in place)
static inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
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

    std::string line;
    while (std::getline(infile, line)) {
        // y=1388, x=546..564
        // x=532, y=716..727

        auto const parts = split(line, ',');
        assert(parts.size() == 2);

        auto const fnum_str = parts[0].substr(2);
        auto const fnum = std::atoi(fnum_str.c_str());

        auto idx = parts[1].find("..");
        assert(idx != std::string::npos);

        auto const snum_str0 = parts[1].substr(3, idx - 3);
        auto const snum0 = std::atoi(snum_str0.c_str());
        auto const snum_str1 = parts[1].substr(idx + 2);
        auto const snum1 = std::atoi(snum_str1.c_str());

        input_values.emplace_back(parts[0][0], fnum, parts[1][1], snum0, snum1);

        // std::cout << input_values.back() << std::endl;
    }

    return input_values;
}

std::tuple<int, int, int, int> get_limits(vec_input const& input_values) {
    int min_x = std::numeric_limits<int>::max();
    int min_y = std::numeric_limits<int>::max();
    int max_x = std::numeric_limits<int>::min();
    int max_y = std::numeric_limits<int>::min();
    std::for_each(std::cbegin(input_values), std::cend(input_values), [&](line const& l) {
        if (l.hv == 'x') {
            min_x = std::min(min_x, l.hv_num);
            max_x = std::max(max_x, l.hv_num);
        } else {
            min_y = std::min(min_y, l.hv_num);
            max_y = std::max(max_y, l.hv_num);
        }

        if (l.hv_range == 'x') {
            min_x = std::min(min_x, l.hv_start);
            max_x = std::max(max_x, l.hv_end);
        } else {
            min_y = std::min(min_y, l.hv_start);
            max_y = std::max(max_y, l.hv_end);
        }
    });
    --min_x;
    ++max_x;
    return std::make_tuple(min_x, max_x, min_y, max_y);
}

void print_map(vec_vec_char const& map) {
    std::for_each(std::cbegin(map), std::cend(map), [](vec_char const& row) {
        std::for_each(std::cbegin(row), std::cend(row), [](char const& cell) { std::cout << cell; });
        std::cout << std::endl;
    });

    std::cout << std::endl;
}

vec_vec_char get_map(vec_input const& input_values) {
    auto [min_x, max_x, min_y, max_y] = get_limits(input_values);
    vec_vec_char map(max_y + 1, vec_char(max_x - min_x + 1, '.'));

    std::for_each(std::cbegin(input_values), std::cend(input_values), [=, &map](line const& l) {
        if (l.hv == 'x') {
            auto const x = l.hv_num - min_x;
            auto const s_y = l.hv_start;
            auto const e_y = l.hv_end + 1;
            std::for_each(std::begin(map) + s_y, std::begin(map) + e_y, [=](vec_char& row) { row[x] = '#'; });
        } else {
            auto const y = l.hv_num;
            auto const s_x = l.hv_start - min_x;
            auto const e_x = l.hv_end - min_x + 1;
            std::for_each(std::begin(map[y]) + s_x, std::begin(map[y]) + e_x, [=](char& cell) { cell = '#'; });
        }
    });
    map[0][500 - min_x] = '+';

    // std::cout << "X: " << min_x << " -> " << max_x << std::endl;
    // std::cout << "Y: " << min_y << " -> " << max_y << std::endl;
    // print_map(map);

    std::set<pos> processed;

    std::vector<pos> end_positions{pos{500 - min_x, 0, Direction::Down}};
    std::vector<pos> new_end_positions;
    auto it = end_positions.cend();
    for (int counter = 0;; ++counter) {
        std::for_each(std::cbegin(end_positions), it, [&](auto const& pp) {
            auto p = pp;
            for (;;) {
                auto const res = processed.insert(p);
                if (!res.second) {
                    return;
                }

                if (p.d == Direction::Down) {
                    if (p.y + 1 == static_cast<int>(map.size())) {
                        return;
                    }

                    if (map[p.y + 1][p.x] == '.' || map[p.y + 1][p.x] == '|') {
                        map[p.y + 1][p.x] = '|';
                        new_end_positions.emplace_back(p.x, p.y + 1, p.d);
                        return;
                    } else {
                        p.d = Direction::Up;
                        continue;
                    }
                } else {
                    int start = p.x;
                    while (start > 0 && map[p.y][start - 1] != '#' && map[p.y + 1][start] != '.') {
                        --start;
                    }

                    int end = p.x;
                    while (end < max_x - min_x && map[p.y][end + 1] != '#' && map[p.y + 1][end] != '.') {
                        ++end;
                    }

                    if (map[p.y][start - 1] == '#' && map[p.y][end + 1] == '#') {
                        std::fill(std::begin(map[p.y]) + start, std::begin(map[p.y]) + end + 1, '~');
                        new_end_positions.emplace_back(p.x, p.y - 1, p.d);
                        return;
                    } else {
                        std::fill(std::begin(map[p.y]) + start, std::begin(map[p.y]) + end + 1, '|');
                        if (map[p.y][start - 1] != '#') {
                            new_end_positions.emplace_back(start, p.y, Direction::Down);
                        }
                        if (map[p.y][end + 1] != '#') {
                            new_end_positions.emplace_back(end, p.y, Direction::Down);
                        }
                        return;
                    }
                }
            }
        });

        new_end_positions.insert(new_end_positions.end(), it, end_positions.cend());
        if (new_end_positions.size() == 0) {
            break;
        }

        std::swap(new_end_positions, end_positions);
        new_end_positions.clear();

        std::sort(std::begin(end_positions), std::end(end_positions));
        end_positions.erase(std::unique(std::begin(end_positions), std::end(end_positions)), std::end(end_positions));
        it = std::find_if(std::cbegin(end_positions), std::cend(end_positions),
                          [&](auto const& elem) { return elem.y != end_positions.front().y; });
    }

    return map;
}

void part1(vec_input const& input_values) {
    auto [min_x, max_x, min_y, max_y] = get_limits(input_values);
    auto const map = get_map(input_values);

    // print_map(map);
    // std::cout << std::endl;

    int sum = std::accumulate(std::cbegin(map) + min_y, std::cend(map), 0, [](auto const& base, auto const& row) {
        return base + std::count_if(std::cbegin(row), std::cend(row), [](char elem){
            return elem == '|' || elem == '~';
        });
    });

    std::cout << "Part1: " << sum << std::endl;
}

void part2(vec_input const& input_values) {
    auto [min_x, max_x, min_y, max_y] = get_limits(input_values);
    auto const map = get_map(input_values);

    // print_map(map);
    // std::cout << std::endl;

    int sum = std::accumulate(std::cbegin(map) + min_y, std::cend(map), 0, [](auto const& base, auto const& row) {
        return base + std::count(std::cbegin(row), std::cend(row), '~');
    });

    std::cout << "Part2: " << sum << std::endl;
}

int main() {
    auto const input_values = read_input("input");

    part1(input_values);
    part2(input_values);

    return 0;
}
