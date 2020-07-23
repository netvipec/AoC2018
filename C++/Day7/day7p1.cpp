#include <bits/stdc++.h>

typedef std::unordered_map<char, std::vector<char>> half_vec_input;
typedef std::pair<half_vec_input, half_vec_input> vec_input;
typedef std::vector<int> vec_int;
typedef std::vector<char> vec_char;

vec_input read_input(std::string const& file_path) {
    vec_input input_values;

    std::ifstream infile(file_path);

    std::string line;
    while (std::getline(infile, line)) {
        char start = line[5];
        char end = line[36];

        input_values.first[start].push_back(end);
        input_values.second[end].push_back(start);
    }

    return input_values;
}

vec_char solution_part1(half_vec_input const& input_values, bool sorted_alphabetically) {
    std::vector<bool> used_letter(26, false);
    vec_int income_edge(26, 0);
    std::for_each(std::cbegin(input_values), std::cend(input_values), [&](auto const& elem){
        used_letter[elem.first - 'A'] = true;
        
        std::for_each(std::cbegin(elem.second), std::cend(elem.second), [&](auto const& elem){
            used_letter[elem - 'A'] = true;
            ++income_edge[elem - 'A'];
        });
    });

    auto const letters_count = std::count(std::cbegin(used_letter), std::cend(used_letter), true);

    vec_char final_letter;
    while (static_cast<int>(final_letter.size()) < letters_count) {
        for (unsigned int i = 0; i < income_edge.size(); ++i) {
            if (used_letter[i] && income_edge[i] == 0) {
                final_letter.push_back('A' + i);
                used_letter[i] = false;

                auto const& children_nodes = input_values.find('A' + i);
                if (children_nodes != std::cend(input_values)) {
                    std::for_each(std::cbegin(children_nodes->second), std::cend(children_nodes->second), [&](auto const& elem){
                        --income_edge[elem - 'A'];
                    });
                }
                if (sorted_alphabetically)
                    break;
            }
        }
    }

    return final_letter;
}

void part1(vec_input const& input_values) {
    auto const final_letter = solution_part1(input_values.first, true);

    std::cout << "Part1: ";
    std::for_each(std::cbegin(final_letter), std::cend(final_letter), [&](auto const& elem){
        std::cout << elem;
    });
    std::cout << std::endl;
}

void part2(vec_input const& input_values) {
    auto const final_letter = solution_part1(input_values.first, false);

    std::for_each(std::cbegin(final_letter), std::cend(final_letter), [&](auto const& elem){
        std::cout << elem;
    });
    std::cout << std::endl;

    constexpr int workers_count = 5;
    constexpr int seconds_base = 60;

    std::vector<std::vector<std::pair<char, int>>> workers(workers_count, std::vector<std::pair<char, int>>{ std::make_pair(0, 0)});
    std::unordered_set<char> finished;

    std::for_each(std::cbegin(final_letter), std::cend(final_letter), [&](auto const& elem){
        auto dependencies_it = input_values.second.find(elem);
        if (dependencies_it == std::cend(input_values.second)) {
            auto best_worker_it = std::min_element(std::begin(workers), std::end(workers), [](auto const& lhs, auto const& rhs){
                return lhs.back().second < rhs.back().second;
            });
            best_worker_it->emplace_back(elem, (elem - 'A' + 1) + seconds_base);
        } else {
            std::vector<std::pair<char, int>> dependencies_final_time;
            std::for_each(std::cbegin(workers), std::cend(workers), [&](auto const& elem){
                std::copy_if(std::cbegin(elem), std::cend(elem), std::back_inserter(dependencies_final_time), [&](auto const& e){
                    return std::cend(dependencies_it->second) != std::find_if(std::cbegin(dependencies_it->second), std::cend(dependencies_it->second), [&](auto const& d){
                        return d == e.first;
                    });
                });
            });

            // std::cout << "elem: " << elem << std::endl;
            // std::for_each(std::cbegin(dependencies_final_time), std::cend(dependencies_final_time), [](auto const& elem){
            //     std::cout << elem.first << "->" << elem.second << ", ";
            // });
            // std::cout << std::endl;

            assert(!dependencies_final_time.empty());

            auto min_begin_time_it = std::max_element(std::cbegin(dependencies_final_time), std::cend(dependencies_final_time), [](auto const& lhs, auto const& rhs){
                return lhs.second < rhs.second;
            });

            assert(min_begin_time_it != std::cend(dependencies_final_time));

            auto best_worker_it = std::min_element(std::begin(workers), std::end(workers), [&](auto const& lhs, auto const& rhs){
                auto lhs_diff = lhs.back().second - min_begin_time_it->second;
                auto rhs_diff = rhs.back().second - min_begin_time_it->second;

                if (lhs_diff == 0 && rhs_diff == 0) {
                    return lhs.size() < rhs.size();
                } else if (lhs_diff == 0) {
                    return true;
                } else if (rhs_diff == 0) {
                    return false;
                } else if ((lhs_diff > 0 && rhs_diff > 0) ||
                           (lhs_diff < 0 && rhs_diff < 0)) {
                    return lhs_diff < rhs_diff;
                } else if (lhs_diff < 0) {
                    return true;
                } else if (rhs_diff < 0) {
                    return false;
                }
                return true; //not reached
            });

            assert(best_worker_it != std::end(workers));

            auto const begin_time = std::max(best_worker_it->back().second, min_begin_time_it->second);
            best_worker_it->emplace_back(elem, begin_time + (elem - 'A' + 1) + seconds_base);
        }
    });

    auto max_it = std::max_element(std::cbegin(workers), std::cend(workers), [](auto const& lhs, auto const& rhs){
        return lhs.back().second < rhs.back().second;
    });
    std::cout << "Part2: " << max_it->back().second << std::endl;

    std::for_each(std::cbegin(workers), std::cend(workers), [](auto const& elem){
        std::cout << "Worker: ";
        std::for_each(std::next(std::cbegin(elem)), std::cend(elem), [](auto const& e){
            std::cout << e.first << "->" << e.second << ",";
        });
        std::cout << std::endl;
    });
}

int main() {
    auto const input_values = read_input("input");
    // std::for_each(std::cbegin(input_values), std::cend(input_values), [](auto const& elem){
    //     std::cout << elem.first << " -> ";
    //     std::for_each(std::cbegin(elem.second), std::cend(elem.second), [](auto const& elem){
    //         std::cout << elem << ",";
    //     });
    //     std::cout << std::endl;
    // });

    part1(input_values);
    part2(input_values);

    return 0;
}