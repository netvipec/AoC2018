#include <bits/stdc++.h>

typedef std::pair<int, int> point_t;
typedef std::vector<point_t> vec_input;
typedef std::vector<int> vec_int;
typedef std::vector<char> vec_char;

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
        auto const p = split(line, ',');
        auto const p1 = p[1].substr(1);

        input_values.emplace_back(std::atoi(p[0].c_str()), std::atoi(p1.c_str()));
    }

    return input_values;
}

int manhattan_distance(point_t const& p1, int p2x, int p2y) {
    return std::abs(p1.first - p2x) + std::abs(p1.second - p2y);
}

char best_distance(vec_input const& input_values, int r, int c) {
    auto const min_it = std::min_element(std::cbegin(input_values), std::cend(input_values), [=](auto const& lhs, auto const& rhs){
        return manhattan_distance(lhs, r, c) < manhattan_distance(rhs, r, c);
    });
    auto const manhattan_distance_min = manhattan_distance(*min_it, r, c);
    auto const min_count = std::count_if(std::cbegin(input_values), std::cend(input_values), [=](auto const& elem){
        return manhattan_distance(elem, r, c) == manhattan_distance_min;
    });
    if (min_count > 1) {
        return 0;
    } else {
        return std::distance(std::cbegin(input_values), min_it) + 1;
    }
}

int calculate_area(std::vector<vec_char> const& map, int v) {
    return std::accumulate(std::cbegin(map), std::cend(map), 0, [=](auto const& base, auto const& elem){
        return base + std::count(std::cbegin(elem), std::cend(elem), v);
    });
}

void part1(vec_input const& input_values) {
    auto const max_it_1 = std::max_element(std::cbegin(input_values), std::cend(input_values), [](auto const& lhs, auto const& rhs){
        return lhs.first < rhs.first;
    });
    auto const max_it_2 = std::max_element(std::cbegin(input_values), std::cend(input_values), [](auto const& lhs, auto const& rhs){
        return lhs.second < rhs.second;
    });
    auto const max_size = std::max(max_it_1->first, max_it_2->second);
    constexpr int extra_size = 10;
    std::vector<vec_char> map(max_size + extra_size, vec_char(max_size + extra_size));

    for (int r = 0; r < static_cast<int>(map.size()); ++r) {
        for (int c = 0; c < static_cast<int>(map.size()); ++c) {
            auto find_it = std::find(std::cbegin(input_values), std::cend(input_values), std::make_pair(r, c));
            if (find_it != std::cend(input_values)) {
                map[r][c] = std::distance(std::cbegin(input_values), find_it) + 1;
            } else {
                map[r][c] = best_distance(input_values, r, c);
            }

            // std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(map[r][c]) << ",";
        }
        // std::cout << std::endl;
    }

    std::vector<bool> removed_values(input_values.size() + 1);
    auto remove_elements = [&](auto const& elem) {
        removed_values[elem] = true;
    };
    std::for_each(std::cbegin(map.front()), std::cend(map.front()), remove_elements);
    std::for_each(std::cbegin(map.back()), std::cend(map.back()), remove_elements);
    for (unsigned int i = 0; i < map.size(); ++i) {
        remove_elements(map[i].front());
        remove_elements(map[i].back());
    }

    // for (unsigned int i = 0; i < map.size(); ++i) {
    //     if (removed_values[i]) {
    //         std::cout << "removed " << i << std::endl;
    //     }
    // }

    int max_area = 0;
    int max_area_index = 0;
    for (int i = 0; i < static_cast<int>(input_values.size()); i++) {
        if (!removed_values[i+1]) {
            auto const area = calculate_area(map, i+1);
            if (area > max_area) {
                max_area = area;
                max_area_index = i;
            }
        }
    }

    std::cout << "Part1: " << max_area << std::endl;
}

void part2(vec_input const& input_values) {
    constexpr int max_accum_manhattan_distance = 10000;
    constexpr int max_size = 2000;
    int counter = 0;
    for (int r = 0; r < max_size; ++r) {
        for (int c = 0; c < max_size; ++c) {
            auto const accumulate_md = std::accumulate(std::cbegin(input_values), std::cend(input_values), 0, [=](auto const& base, auto const& elem){
                return base + manhattan_distance(elem, r, c);
            });

            if (accumulate_md < max_accum_manhattan_distance) {
                ++counter;
            }
        }
    }

    std::cout << "Part2: " << counter << std::endl;
}

int main() {
    auto const input_values = read_input("input");
    // std::for_each(std::cbegin(input_values), std::cend(input_values), [](auto const& elem){
    //     std::cout << elem.first << "," << elem.second << std::endl;
    // });

    part1(input_values);
    part2(input_values);

    return 0;
}