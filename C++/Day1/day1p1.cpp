#include <bits/stdc++.h>

typedef std::vector<int> vec_int;

vec_int read_input(std::string const& file_path) {
    vec_int input_values;

    std::ifstream infile(file_path);

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        int a;
        if (!(iss >> a)) {
            break;
        } // error

        input_values.push_back(a);
    }
    return input_values;
}

int apply_until_repeated(vec_int const& input_values) {
    std::unordered_set<int> sums;

    int sum = 0;
    for (;;) {
        for (auto const& v : input_values) {
            sum += v;
            auto const res = sums.insert(sum);
            if (!res.second) {
            	return sum;
            }
        }
    }
    return 0;
}

int main() {
    auto const input_values = read_input("input");
    // std::for_each(std::cbegin(input_values), std::cend(input_values), [](auto const& elem){ std::cout << elem << ", "; });
    // std::cout << std::endl;

    auto const sum = std::reduce(std::cbegin(input_values), std::cend(input_values));
    std::cout << "Part1: " << sum << std::endl;

    auto const sum2 = apply_until_repeated(input_values);
    std::cout << "Part2: " << sum2 << std::endl;

    return 0;
}