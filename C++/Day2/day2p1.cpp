#include <bits/stdc++.h>

typedef std::vector<std::string> vec_string;
typedef std::vector<int> vec_int;

vec_string read_input(std::string const& file_path) {
    vec_string input_values;

    std::ifstream infile(file_path);

    std::string line;
    while (std::getline(infile, line)) {
        input_values.push_back(line);
    }
    return input_values;
}

void part1(vec_string const& input_values) {
    int box_letter_repeated_2 = 0;
    int box_letter_repeated_3 = 0;

    std::for_each(std::cbegin(input_values), std::cend(input_values), [&](auto const& box_id){
        vec_int values(26, 0);
        std::for_each(std::cbegin(box_id), std::cend(box_id), [&](auto const& box_letter){
            ++values[box_letter - 'a'];
        });

        auto two_it = std::find(std::cbegin(values), std::cend(values), 2);
        auto three_it = std::find(std::cbegin(values), std::cend(values), 3);

        box_letter_repeated_2 += (two_it != std::cend(values)) ? 1 : 0;
        box_letter_repeated_3 += (three_it != std::cend(values)) ? 1 : 0;
    });

    std::cout << "Two  : " << box_letter_repeated_2 << std::endl;
    std::cout << "Three: " << box_letter_repeated_3 << std::endl;
    std::cout << "Part1: " << box_letter_repeated_2 * box_letter_repeated_3 << std::endl;
}

void part2(vec_string const& input_values) {
    for (auto it_left = std::cbegin(input_values), it_end = std::cend(input_values); it_left != it_end; ++it_left) {
        for (auto it_right = std::next(it_left); it_right != it_end; ++it_right) {
            auto const mismatch_its = std::mismatch(std::cbegin(*it_left), std::cend(*it_left), std::cbegin(*it_right));
            assert(mismatch_its.first != std::cend(*it_left));
            assert(mismatch_its.second != std::cend(*it_right));

            auto const mismatch_rest_its = std::mismatch(std::next(mismatch_its.first), std::cend(*it_left), std::next(mismatch_its.second));
            if (mismatch_rest_its.first == std::cend(*it_left) && mismatch_rest_its.second == std::cend(*it_right)) {
                std::string res(std::cbegin(*it_left), mismatch_its.first);
                res.append(std::next(mismatch_its.first), std::cend(*it_left));
                std::cout << "Part2: " << res << std::endl;
                return;
            }
        }
    }
}

int main() {
    auto const input_values = read_input("input");
    // std::for_each(std::cbegin(input_values), std::cend(input_values), [](auto const& elem){ std::cout << elem << ", "; });
    // std::cout << std::endl;

    part1(input_values);
    part2(input_values);

    return 0;
}