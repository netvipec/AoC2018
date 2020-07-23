#include <bits/stdc++.h>

typedef std::vector<int64_t> vec_int;
typedef int64_t vec_input;

vec_input read_input(std::string const&) {
    return 894501;
}

void add_digits(vec_int& recipes, int64_t recipe_id) {
    if (recipe_id == 0) {
        recipes.push_back(0);
        return;
    }
    vec_int digits;
    while (recipe_id > 0) {
        digits.push_back(recipe_id % 10);
        recipe_id /= 10;
    }

    recipes.insert(recipes.end(), digits.crbegin(), digits.crend());
}

void print_recipes(vec_int const& recipes, int actual_index1, int actual_index2) {
    for (int i = 0, i_end = recipes.size(); i < i_end; ++i) {
        if (i == actual_index1) {
            std::cout << "(" << recipes[i] << ")";
        } else if (i == actual_index2) {
            std::cout << "[" << recipes[i] << "]";
        } else {
            std::cout << " " << recipes[i] << " ";
        }
    }
    std::cout << std::endl;
}

void part1(vec_input const& input_values) {
    auto const final_value = input_values + 10;

    vec_int recipes { 3, 7 };
    int actual_index1 = 0;
    int actual_index2 = 1;

    // print_recipes(recipes, actual_index1, actual_index2);

    for (;;) {
        add_digits(recipes, recipes[actual_index1] + recipes[actual_index2]);
        actual_index1 = (actual_index1 + recipes[actual_index1] + 1) % recipes.size();
        actual_index2 = (actual_index2 + recipes[actual_index2] + 1) % recipes.size();

        // print_recipes(recipes, actual_index1, actual_index2);

        if (static_cast<int>(recipes.size()) >= final_value) {
            break;
        }
    }

    std::cout << "Part1: ";
    std::for_each(std::cbegin(recipes) + input_values, std::cbegin(recipes) + final_value, [](auto const& elem){
            std::cout << elem;
    });
    std::cout << std::endl;
}

void part2(vec_input const& input_values) {
    vec_int to_found;
    add_digits(to_found, input_values);
    // print_recipes(to_found, 0, 1);

    vec_int recipes { 3, 7 };
    int actual_index1 = 0;
    int actual_index2 = 1;
    int last_search = 0;

    // print_recipes(recipes, actual_index1, actual_index2);

    for (;;) {
        add_digits(recipes, recipes[actual_index1] + recipes[actual_index2]);
        actual_index1 = (actual_index1 + recipes[actual_index1] + 1) % recipes.size();
        actual_index2 = (actual_index2 + recipes[actual_index2] + 1) % recipes.size();

        // print_recipes(recipes, last_search, actual_index2);

        if (last_search + to_found.size() <= recipes.size()) {
            auto const it = std::search(std::cbegin(recipes) + last_search, std::cend(recipes), std::cbegin(to_found), std::cend(to_found));
            if (it != std::cend(recipes)) {
                std::cout << "Part2: " << std::distance(std::cbegin(recipes), it) << std::endl;
                break;
            }

            last_search = recipes.size() - to_found.size();
        }
    }
}

int main() {
    auto const input_values = read_input("input");
    // std::cout << "Map: " << std::endl;
    // std::for_each(std::cbegin(input_values), std::cend(input_values), [](auto const& elem){
    //     std::for_each(std::cbegin(elem), std::cend(elem), [](auto const& e){
    //         std::cout << e;
    //     });
    //     std::cout << std::endl;
    // });
    // std::cout << std::endl;
    
    part1(input_values);
    part2(input_values);

    return 0;
}