#include <bits/stdc++.h>

typedef std::vector<char> vec_input;
typedef std::vector<int> vec_int;

vec_input read_input(std::string const& file_path) {
    vec_input input_values;

    std::ifstream infile(file_path);

    std::string line;
    while (std::getline(infile, line)) {
        input_values.assign(std::cbegin(line), std::cend(line));
    }

    return input_values;
}

vec_input collapse_polymer(vec_input const& input_values) {
    auto iv = input_values;
    for (;;) {
        auto it = std::adjacent_find(std::cbegin(iv), std::cend(iv), [](auto const& lhs, auto const& rhs){
            return (lhs ^ rhs) == 32;
        });

        if (it == std::cend(iv)) {
            break;
        }

        if (it == std::cbegin(iv)) {
            it = iv.erase(it, it + 2);
        } else {
            it = iv.erase(it, it + 2);
            it = std::prev(it);
        }
    }
    return iv;
}

void part1(vec_input const& input_values) {
    auto iv = collapse_polymer(input_values);

    std::cout << "Part1: " << iv.size() << std::endl;
}

void part2(vec_input const& input_values) {
    vec_input iv;
    vec_int best_letter(26);
    for (int i = 0; i < 26; ++i) {
        iv.clear();
        std::copy_if(std::cbegin(input_values), std::cend(input_values), std::back_inserter(iv), [=](auto const& elem){
            return (elem ^ ('A' + i)) != 0 && (elem ^ ('A' + i)) != 32;
        });

        auto const ivc = collapse_polymer(iv);
        best_letter[i] = ivc.size();
    }

    auto max_it = std::min_element(std::cbegin(best_letter), std::cend(best_letter));
    std::cout << "Best letter: " << static_cast<char>('A' + std::distance(std::cbegin(best_letter), max_it)) << ", with size: " << *max_it << std::endl;
    std::cout << "Part2: " << *max_it << std::endl;
}

int main() {
    auto const input_values = read_input("input");
    // std::for_each(std::cbegin(input_values), std::cend(input_values), [](auto const& elem){
    //     std::cout << elem;
    // });
    // std::cout << std::endl;

    part1(input_values);
    part2(input_values);

    return 0;
}