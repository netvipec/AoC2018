#include <bits/stdc++.h>

typedef std::vector<int64_t> vec_int;
typedef std::pair<std::string, std::vector<std::pair<std::string, std::string>>> vec_input;

vec_input read_input(std::string const& file_path) {
    vec_input input_values;

    std::ifstream infile(file_path);

    std::string line;
    std::getline(infile, line);
    input_values.first = line.substr(15);

    std::getline(infile, line);
    while (std::getline(infile, line)) {
        auto const separator_index = line.find("=>");
        assert(separator_index != std::string::npos);

        auto const state = line.substr(0, separator_index - 1);
        auto const result = line.substr(separator_index + 3);

        input_values.second.emplace_back(state, result);
    }

    return input_values;
}

int read_number(std::string::const_iterator number_str_it, std::string::const_iterator number_str_it_end) {
    int number = 0;
    std::for_each(number_str_it, number_str_it_end, [&](auto const& elem){
        number = 2 * number + ((elem == '#') ? 1 : 0);
    });
    return number;
}

std::tuple<vec_int, int> evolve(vec_int const& initial_state, int begin_index, std::unordered_map<int, int> const& states) {
    vec_int result;
    result.reserve(initial_state.size() + 10);

    int mask = (1 << 5) - 1;

    int new_begin_index = begin_index;

    int number = 0;
    for (int i = -5; i < static_cast<int>(initial_state.size()) + 5; ++i) {
        auto const elem = (i < 0) ? 0 : (i >= static_cast<int>(initial_state.size()) ? 0 : initial_state[i]);

        number = 2 * number + elem;
        number &= mask;

        auto it = states.find(number);
        if (it != std::cend(states)) {
            if (!result.empty() || it->second != 0) {
                if (result.empty()) {
                    new_begin_index = begin_index + i - 2;
                }
                result.push_back(it->second);
            }
        } else if (!result.empty()) {
            result.push_back(0);
        }
    }

    auto it = std::find(std::crbegin(result), std::crend(result), 1).base();
    result.erase(it, std::cend(result));

    return std::make_tuple(result, new_begin_index);   
}

void part1(vec_input const& input_values) {
    vec_int initial_state;
    initial_state.reserve(input_values.first.size());
    std::transform(std::cbegin(input_values.first), std::cend(input_values.first), std::back_inserter(initial_state), [](auto const& elem){
        return (elem == '#') ? 1 : 0;
    });

    std::unordered_map<int, int> states;
    std::for_each(std::cbegin(input_values.second), std::cend(input_values.second), [&](auto const& elem){
        auto const key = read_number(elem.first.cbegin(), elem.first.cend());
        auto const value = (elem.second == "#") ? 1 : 0;

        states.insert(std::make_pair(key, value));
    });

    int begin_index = 0;
    for (int generations = 0; generations < 20; ++generations) {
        std::tie(initial_state, begin_index) = evolve(initial_state, begin_index, states);

        // std::cout << "generation " << generations << "(" << initial_state.size() << "," << begin_index << "): ";
        // std::for_each(std::cbegin(initial_state), std::cend(initial_state), [](auto const& elem){
        //     std::cout << (elem ? '#' : '.');
        // });
        // std::cout << std::endl;
    }

    int sum = 0;
    for (int i = 0; i < static_cast<int>(initial_state.size()); ++i) {
        if (initial_state[i]) {
            sum += begin_index + i;
            // std::cout << begin_index + i << ",";
        }
    }
    // std::cout << std::endl;

    std::cout << "Part1: " << sum << std::endl;
}

void part2(vec_input const& input_values) {
    vec_int initial_state;
    initial_state.reserve(input_values.first.size());
    std::transform(std::cbegin(input_values.first), std::cend(input_values.first), std::back_inserter(initial_state), [](auto const& elem){
        return (elem == '#') ? 1 : 0;
    });

    std::unordered_map<int, int> states;
    std::for_each(std::cbegin(input_values.second), std::cend(input_values.second), [&](auto const& elem){
        auto const key = read_number(elem.first.cbegin(), elem.first.cend());
        auto const value = (elem.second == "#") ? 1 : 0;

        states.insert(std::make_pair(key, value));
    });

    // The state repeat after 110 generations, maintaining the size increasing the begin_index by 1 from 36.
    // that mean that in generation 200, the state have the same shape that in generation 110 only starting from index 126 = 200 - 110 + 36

    constexpr int64_t end_generations = 50000000000ull - 1;
    // constexpr int64_t end_generations = 180;
    constexpr int64_t quantity_of_plant = 86;
    constexpr int64_t begin_index_generation = end_generations - 110 + 36;
    constexpr int64_t end_index_generation = begin_index_generation + 2 * quantity_of_plant;
    constexpr int64_t final_result = ((begin_index_generation + end_index_generation) * (quantity_of_plant + 1)) / 2;
    std::cout << begin_index_generation << "," << end_index_generation << "," << final_result << std::endl;
    // int begin_index = 0;
    // for (int generations = 0; generations < 181; ++generations) {
    //     std::tie(initial_state, begin_index) = evolve(initial_state, begin_index, states);

    //     std::cout << "generation " << generations << "(" << initial_state.size() << "," << begin_index << "): ";
    //     std::for_each(std::cbegin(initial_state), std::cend(initial_state), [](auto const& elem){
    //         std::cout << (elem ? '#' : '.');
    //     });
    //     std::cout << std::endl;
    // }

    // int sum = 0;
    // for (int i = 0; i < static_cast<int>(initial_state.size()); ++i) {
    //     if (initial_state[i]) {
    //         sum += begin_index + i;
    //         // std::cout << begin_index + i << ",";
    //     }
    // }
    // std::cout << std::endl;
    // std::cout << "Part2: " << sum << std::endl;

    std::cout << "Part2: " << final_result << std::endl;
}

int main() {
    auto const input_values = read_input("input");
    // std::cout << "Initial state: " << input_values.first << std::endl << std::endl;
    // std::for_each(std::cbegin(input_values.second), std::cend(input_values.second), [](auto const& elem){
    //     std::cout << elem.first << " => " << elem.second << std::endl;
    // });
    // std::cout << std::endl;
    
    part1(input_values);
    part2(input_values);

    return 0;
}