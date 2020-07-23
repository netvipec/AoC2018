#include <bits/stdc++.h>

// up, right, down, left
constexpr std::array<char, 4> dir_symbol = { '^', '>', 'v', '<' };
constexpr std::array<std::pair<int, int>, 4> dir_diff = {std::make_pair(-1, 0), std::make_pair(0, 1), std::make_pair(1, 0), std::make_pair(0, -1)};

struct car {
    int x = 0;
    int y = 0;
    int dir = 0;
    int turn_index = 0;

    car(int xx, int yy, int dire)
        : x(xx), y(yy), dir(dire) {}

    bool operator<(car const& other) const {
        return std::tie(x, y, dir, turn_index) < std::tie(other.x, other.y, other.dir, turn_index);
    }
};

typedef std::vector<int64_t> vec_int;
typedef std::vector<std::vector<char>> vec_input;
typedef std::vector<car> car_t;

vec_input read_input(std::string const& file_path) {
    vec_input input_values;

    std::ifstream infile(file_path);

    std::string line;
    while (std::getline(infile, line)) {
        input_values.emplace_back(std::vector<char>(std::cbegin(line), std::cend(line)));
    }

    return input_values;
}

car_t get_car_pos(vec_input const& map) {
    car_t cars;
    for (int x = 0, x_end = map.size(); x < x_end; ++x) {
        for (int y = 0, y_end = map[x].size(); y < y_end; ++y) {
            auto v = map[x][y];
            auto const it = std::find(std::cbegin(dir_symbol), std::cend(dir_symbol), v);
            if (it != std::cend(dir_symbol)) {
                cars.emplace_back(x, y, static_cast<int>(std::distance(std::cbegin(dir_symbol), it)));
            }
        }
    }
    return cars;
}

std::optional<std::pair<int, int>> update_cars(car_t& cars, vec_input const& map) {
    std::optional<std::pair<int, int>> found_car = std::nullopt;

    int counter = 0;
    std::for_each(std::begin(cars), std::end(cars), [&](auto& car){
        auto const new_x = car.x + dir_diff[car.dir].first;
        auto const new_y = car.y + dir_diff[car.dir].second;

        auto new_dir = -1;
        auto new_turn_index = car.turn_index;
        switch (map[new_x][new_y]) {
            case '^':
            case '>':
            case '<':
            case 'v':
            case '-':
            case '|':
                new_dir = car.dir;
                break;
            case '+':
                switch (car.turn_index % 3) {
                    case 0:
                        new_dir = (car.dir + 3) % 4;
                        break;
                    case 1:
                        new_dir = car.dir;
                        break;
                    case 2:
                        new_dir = (car.dir + 1) % 4;
                        break;
                }
                ++new_turn_index;
                break;
            case '/':
                if (car.dir < 2) {
                    new_dir = (car.dir == 0 ? 1 : 0);
                } else {
                    new_dir = (car.dir == 2 ? 3 : 2);
                }
                break;
            case '\\':
                if (car.dir == 0 || car.dir == 3) {
                    new_dir = (car.dir == 0 ? 3 : 0);
                } else {
                    new_dir = (car.dir == 2 ? 1 : 2);
                }
                break;
            default:
                std::cout << "Error in map: " << new_x << "," << new_y << " with value: " << map[new_x][new_y] << std::endl;
                break;
        }

        car.x = new_x;
        car.y = new_y;
        car.dir = new_dir;
        car.turn_index = new_turn_index;

        auto const crash_car = std::find_if(std::cbegin(cars) + counter + 1, std::cend(cars), [=](auto const& elem){
            return std::tie(elem.x, elem.y) == std::tie(new_x, new_y);
        });
        if (crash_car != std::cend(cars)) {
            found_car = std::make_pair(counter, std::distance(std::cbegin(cars), crash_car));
        }
        ++counter;
    });

    return found_car;
}

void part1(vec_input const& input_values) {
    auto cars = get_car_pos(input_values);

    // std::cout << "Cars: " << std::endl;
    // std::for_each(std::cbegin(cars), std::cend(cars), [](auto const& elem){
    //     std::cout << elem.x << "," << elem.y << " -> " << elem.dir << std::endl;
    // });

    int counter = 0;
    for (;;) {
        ++counter;
        auto const crash_car = update_cars(cars, input_values);

        if (crash_car.has_value()) {
            std::cout << "Part1: " << cars[crash_car->first].y << "," << cars[crash_car->first].x << std::endl;
            break;
        }

        std::sort(std::begin(cars), std::end(cars));

        auto it = std::adjacent_find(std::cbegin(cars), std::cend(cars), [](auto const& lhs, auto const& rhs){
            return std::tie(lhs.x, lhs.y) == std::tie(rhs.x, rhs.y);
        });

        // std::cout << "Cars [" << counter << "]: " << std::endl;
        // std::for_each(std::cbegin(cars), std::cend(cars), [](auto const& elem){
        //     std::cout << elem.x << "," << elem.y << " -> " << elem.dir << "," << elem.turn_index << std::endl;
        // });
        // std::cout << std::endl;

        if (it != std::cend(cars)) {
            std::cout << "Part1: " << it->y << "," << it->x << std::endl;
            break;
        }
    }
}

void part2(vec_input const& input_values) {
    auto cars = get_car_pos(input_values);

    int counter = 0;
    for (;;) {
        ++counter;
        auto const crash_car = update_cars(cars, input_values);

        if (crash_car.has_value()) {
            cars.erase(cars.begin() + crash_car->second);
            cars.erase(cars.begin() + crash_car->first);
        }

        std::sort(std::begin(cars), std::end(cars));

        auto it = std::adjacent_find(std::cbegin(cars), std::cend(cars), [](auto const& lhs, auto const& rhs){
            return std::tie(lhs.x, lhs.y) == std::tie(rhs.x, rhs.y);
        });

        if (it != std::cend(cars)) {
            it = cars.erase(it);
            cars.erase(it);
        }

        if (cars.size() == 1) {
            std::cout << "Part2: " << cars.front().y << "," << cars.front().x << std::endl;
            break;
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