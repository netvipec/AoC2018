#include <bits/stdc++.h>

typedef std::vector<int64_t> vec_int;
typedef std::vector<char> vec_char;
typedef std::pair<int64_t, int64_t> vec_input;

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
    std::getline(infile, line);

    auto const values_str = split(line, ';');    
    assert(values_str.size() == 2);

    auto const players_str = split(values_str[0], ' ');
    assert(players_str.size() == 2);

    auto const players = std::atoi(players_str[0].c_str());

    auto const points_str = split(values_str[1], ' ');
    assert(points_str.size() == 7);

    auto const points = std::atoi(points_str[5].c_str());

    return std::make_pair(players, points);
}

void part1(vec_input const& input_values) {
    constexpr int64_t multiple = 23;
    constexpr int64_t prev_value = 7;

    auto const total_points = input_values.second;
    auto const total_players = input_values.first;

    int64_t current_index = 3;
    int64_t current_value = 4;
    int64_t next_multiple = multiple;
    vec_int table{0, 2, 1, 3};
    vec_int players_score(total_players, 0);


    while (current_index < total_points) {
        for (; current_value < next_multiple; ++current_value) {
            // if (current_value < 100) {
            //     std::cout << "Table " << current_value << ": ";
            //     std::for_each(std::cbegin(table), std::cend(table), [](auto const& elem){
            //         std::cout << elem << ",";
            //     });
            //     std::cout << std::endl;
            // }

            auto new_index = (current_index + 2);
            new_index = (new_index > static_cast<int>(table.size())) ? new_index % table.size() : new_index;
            table.insert(std::begin(table) + new_index, current_value);
            current_index = new_index;
        }

        if (current_value <= total_points) {
            players_score[next_multiple % players_score.size()] += next_multiple;
            auto new_index = (current_index + table.size() - prev_value) % table.size();
            players_score[next_multiple % players_score.size()] += table[new_index];
            current_index = new_index;
            ++current_value;
            table.erase(std::begin(table) + new_index);
        }

        next_multiple += multiple;
    }

    
    auto max_it = std::max_element(std::cbegin(players_score), std::cend(players_score), [](auto const& lhs, auto const& rhs){
        return lhs < rhs;
    });
    std::cout << "Part1: " << *max_it << std::endl;
}

void part2(vec_input const& input_values) {
    constexpr int64_t multiple = 23;
    constexpr int64_t prev_value = 7;

    auto const total_points = input_values.second * 100;
    auto const total_players = input_values.first;

    int64_t current_value = 4;    
    std::list<int64_t> table{0, 2, 1, 3};
    auto current_it = std::prev(table.cend());
    vec_int players_score(total_players);


    for (; current_value < total_points; ++current_value) {
        if ((current_value % multiple) == 0) {
            players_score[current_value % total_players] += current_value;

            for (int i = 0; i < prev_value; ++i) {
                std::advance(current_it, -1);
                if (current_it == table.begin()) {
                    current_it = table.end();
                }
            }

            players_score[current_value % total_players] += *current_it;
            current_it = table.erase(current_it);
        } else {
            // if (current_value < 100) {
            //     std::cout << "Table " << current_value << ": ";
            //     std::for_each(std::cbegin(table), std::cend(table), [](auto const& elem){
            //         std::cout << elem << ",";
            //     });
            //     std::cout << std::endl;
            // }

            if (current_it == table.end())
                 current_it = table.begin();
            std::advance(current_it, 1);
            if (current_it == table.end())
                 current_it = table.begin();
            std::advance(current_it, 1);

            current_it = table.insert(current_it, current_value);            
        }
    }

    auto max_it = std::max_element(std::cbegin(players_score), std::cend(players_score), [](auto const& lhs, auto const& rhs){
        return lhs < rhs;
    });
    std::cout << "Part2: " << *max_it << std::endl;
}

int main() {
    auto const input_values = read_input("input");
    std::cout << "Players: " << input_values.first << ", Points: " << input_values.second << std::endl;

    part1(input_values);
    part2(input_values);

    return 0;
}