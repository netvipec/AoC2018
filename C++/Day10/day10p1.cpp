#include <bits/stdc++.h>

typedef std::vector<std::pair<int, int>> vec_pair;
typedef std::pair<vec_pair, vec_pair> vec_input;

std::tuple<int, int> read_number(std::string const& line, int start_offset, int begin_offset, std::string delim) {
    auto const end = line.find(delim, start_offset);
    assert(end != std::string::npos);

    std::string number;
    if (line[begin_offset] == '-') {
        number = line.substr(begin_offset, end);
    } else {
        number = line.substr(begin_offset + 1, end);
    }

    return std::make_tuple(std::atoi(number.c_str()), end);
}

vec_input read_input(std::string const& file_path) {
    vec_input input_values;

    std::ifstream infile(file_path);

    std::string line;
    while (std::getline(infile, line)) {
        auto const [pos_x, end_first] = read_number(line, 0, 9, ",");
        auto const [pos_y, end_second] = read_number(line, end_first, end_first + 1, ">");
        auto const [vel_x, end_third] = read_number(line, end_second, end_second + 11, ",");
        auto const [vel_y, end_fourth] = read_number(line, end_third, end_third + 2, ">");

        // std::cout << pos_x << "," << pos_y << std::endl;
        // std::cout << vel_x << "," << vel_y << std::endl;

        input_values.first.emplace_back(pos_x, pos_y);
        input_values.second.emplace_back(vel_x, vel_y);
    }

    return input_values;
}

void print_points(vec_pair points) {
    std::sort(std::begin(points), std::end(points), [](auto const& lhs, auto const& rhs){
        return std::make_tuple(lhs.second, lhs.first) < std::make_tuple(rhs.second, rhs.first);
    });

    auto last = std::unique(std::begin(points), std::end(points));
    points.erase(last, std::end(points)); 

    auto min_x_it = std::min_element(std::cbegin(points), std::cend(points), [](auto const& lhs, auto const& rhs){
        return lhs.first < rhs.first;
    });
    auto min_y_it = std::min_element(std::cbegin(points), std::cend(points), [](auto const& lhs, auto const& rhs){
        return lhs.second < rhs.second;
    });

    // std::for_each(std::cbegin(points), std::cend(points), [&](auto const& elem){
    //     std::cout << elem.first << "," << elem.second << std::endl;
    // });
    // std::cout << std::endl;

    int last_x = min_x_it->first;
    int last_y = min_y_it->second;
    std::cout << "Part1: " << std::endl;
    std::cout << "min: " << last_x << "," << last_y << std::endl;
    std::for_each(std::cbegin(points), std::cend(points), [&](auto const& elem){
        if (last_y < elem.second) {
            for (int y = last_y; y < elem.second; ++y) {
                std::cout << "\n";
            }
            last_x = min_x_it->first;
        }
        if (last_x < elem.first) {
            for (int x = last_x; x < elem.first; ++x) {
                std::cout << " ";
            }
        }
        last_y = elem.second;
        last_x = elem.first + 1;

        std::cout << "0";
    });
}

void part1(vec_input const& input_values) {
    auto points = input_values.first;
    auto const& velocities = input_values.second;

    // print_points(points);
    // std::cout << std::endl << std::endl;

    constexpr int max_diff_y = 10;
    constexpr int print_max_count = 1;

    int counter = 0;
    int print_count = 0;
    // int best_diff = std::numeric_limits<int>::max();
    for (;;) {
        ++counter;
        for (unsigned int i = 0; i < points.size(); ++i) {
            // std::cout << "pp: (" << points[i].first << "," << points[i].second << ") -> v: (" << velocities[i].first << "," << velocities[i].second << ") -> p: (";
            points[i].first += velocities[i].first;
            points[i].second += velocities[i].second;
            // std::cout << points[i].first << "," << points[i].second << ")" << std::endl;
        }

        auto y_limit_it = std::minmax_element(std::cbegin(points), std::cend(points), [](auto const& lhs, auto const& rhs){ return lhs.second < rhs.second; });

        auto diff = y_limit_it.second->second - y_limit_it.first->second;
        // if (diff < best_diff) {
        //     best_diff = diff;
        //     std::cout << "y: " << min_y_it->second << "," << max_y_it->second << ", diff: " << diff << std::endl;
        // }

        if (diff < max_diff_y) {
            print_points(points);
            std::cout << std::endl << std::endl;
            ++print_count;
            if (print_count >= print_max_count) {
                std::cout << "Part2: " << counter << std::endl;
                break;
            }
        }
    }
}

int main() {
    auto const input_values = read_input("input");
    
    part1(input_values);

    return 0;
}