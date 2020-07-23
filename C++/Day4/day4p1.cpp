#include <bits/stdc++.h>

enum class Operation { GUARD_START, FALL_ASLEEP, WAKES_UP };

typedef std::tm tp_t;
typedef std::vector<std::pair<tp_t, std::pair<Operation, int>>> vec_input;
typedef std::vector<int> vec_int;

// trim from start (in place)
static inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}

static std::vector<std::string> split(std::string const& s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

auto parse_date_time(std::string const& line) {
    std::tm tm = {};
    std::stringstream ss(line);
    ss >> std::get_time(&tm, "[%Y-%m-%d %H:%M]");
    return tm;
}

vec_input read_input(std::string const& file_path) {
    vec_input input_values;

    std::ifstream infile(file_path);

    std::string line;
    while (std::getline(infile, line)) {
        auto dt = parse_date_time(line);

        auto const ls = split(line, ']');
        auto operation_str = ls[1];
        trim(operation_str);

        assert(!operation_str.empty());

        if (operation_str[0] == 'f') {
            input_values.emplace_back(dt, std::make_pair(Operation::FALL_ASLEEP, -1));
        } else if (operation_str[0] == 'w') {
            input_values.emplace_back(dt, std::make_pair(Operation::WAKES_UP, -1));
        } else {
            auto it_start = std::find(std::cbegin(operation_str), std::cend(operation_str), '#');
            assert(it_start != std::cend(operation_str));
            auto it_end = std::find(it_start, std::cend(operation_str), ' ');
            assert(it_end != std::cend(operation_str));

            std::string guard_id(std::next(it_start), it_end);
            input_values.emplace_back(dt, std::make_pair(Operation::GUARD_START, std::atoi(guard_id.c_str())));
        }
    }

    std::sort(std::begin(input_values), std::end(input_values), [](auto const& lhs, auto const& rhs) {
        return std::make_tuple(lhs.first.tm_year, lhs.first.tm_mon, lhs.first.tm_mday, lhs.first.tm_hour, lhs.first.tm_min) <
               std::make_tuple(rhs.first.tm_year, rhs.first.tm_mon, rhs.first.tm_mday, rhs.first.tm_hour, rhs.first.tm_min);
    });

    return input_values;
}

void part1(vec_input const& input_values) {
    std::unordered_map<int, int> guard_id_max_asleep;

    int guard_id = -1;
    int asleep_min = -1;
    std::for_each(std::cbegin(input_values), std::cend(input_values), [&](auto const& elem){
        switch (elem.second.first) {
            case Operation::GUARD_START:
                assert(asleep_min == -1);

                guard_id = elem.second.second;
                asleep_min = -1;
                break;
            case Operation::FALL_ASLEEP:
                asleep_min = elem.first.tm_min;
                break;
            case Operation::WAKES_UP:
                assert(guard_id != -1);
                assert(asleep_min != -1);

                guard_id_max_asleep[guard_id] += elem.first.tm_min - asleep_min;
                asleep_min = -1;
                break;
        }
    });

    auto it = std::max_element(std::cbegin(guard_id_max_asleep), std::cend(guard_id_max_asleep), [](auto const& lhs, auto const& rhs){
        return lhs.second < rhs.second;
    });
    assert(it != std::cend(guard_id_max_asleep));

    // std::for_each(std::cbegin(guard_id_max_asleep), std::cend(guard_id_max_asleep), [](auto const& elem){
    //     std::cout << elem.first << ": " << elem.second << std::endl;
    // });

    auto const guard_id_more_asleep = it->first;
    std::cout << "Guard more asleep " << guard_id_more_asleep << ", time asleep: " << it->second << std::endl;

    std::vector<int> min_asleep(60);

    guard_id = -1;
    asleep_min = -1;
    std::for_each(std::cbegin(input_values), std::cend(input_values), [&](auto const& elem){
        switch (elem.second.first) {
            case Operation::GUARD_START:
                assert(asleep_min == -1);

                guard_id = elem.second.second;
                asleep_min = -1;
                break;
            case Operation::FALL_ASLEEP:
                asleep_min = elem.first.tm_min;
                break;
            case Operation::WAKES_UP:
                assert(guard_id != -1);
                assert(asleep_min != -1);

                if (guard_id == guard_id_more_asleep) {
                    std::transform(std::cbegin(min_asleep) + asleep_min, 
                                   std::cbegin(min_asleep) + elem.first.tm_min,
                                   std::begin(min_asleep) + asleep_min, [](auto const& elem) { return elem + 1; });
                }
                asleep_min = -1;
                break;
        }
    });

    auto it2 = std::max_element(std::cbegin(min_asleep), std::cend(min_asleep));
    auto const min_more_asleep = std::distance(std::cbegin(min_asleep), it2);
    std::cout << "Min of more asleep: " << min_more_asleep << std::endl;
    std::cout << "Part1: " << guard_id_more_asleep * min_more_asleep << std::endl;
}

void part2(vec_input const& input_values) {
    std::unordered_map<int, std::vector<int>> min_asleep(60);

    int guard_id = -1;
    int asleep_min = -1;
    std::for_each(std::cbegin(input_values), std::cend(input_values), [&](auto const& elem){
        switch (elem.second.first) {
            case Operation::GUARD_START:
                assert(asleep_min == -1);

                guard_id = elem.second.second;
                asleep_min = -1;
                break;
            case Operation::FALL_ASLEEP:
                asleep_min = elem.first.tm_min;
                break;
            case Operation::WAKES_UP:
                assert(guard_id != -1);
                assert(asleep_min != -1);

                auto it = min_asleep.find(guard_id);
                if (it == std::cend(min_asleep)) {
                    it = min_asleep.insert(std::make_pair(guard_id, std::vector<int>(60))).first;
                }
                std::transform(std::cbegin(it->second) + asleep_min, 
                               std::cbegin(it->second) + elem.first.tm_min,
                               std::begin(it->second) + asleep_min, [](auto const& elem) { return elem + 1; });
                asleep_min = -1;
                break;
        }
    });

    std::map<int, std::pair<int, int>> guard_min_max_asleep;
    std::for_each(std::cbegin(min_asleep), std::cend(min_asleep), [&](auto const& elem){
        auto it = std::max_element(std::cbegin(elem.second), std::cend(elem.second));
        assert(it != std::cend(elem.second));
        guard_min_max_asleep.insert(std::make_pair(elem.first, std::make_pair(std::distance(std::cbegin(elem.second), it), *it)));
    });

    // std::for_each(std::cbegin(guard_min_max_asleep), std::cend(guard_min_max_asleep), [](auto const& elem){
    //     std::cout << elem.first << ": " << elem.second.first << "," << elem.second.second << std::endl;
    // });
    
    auto it2 = std::max_element(std::cbegin(guard_min_max_asleep), std::cend(guard_min_max_asleep), [](auto const& lhs, auto const& rhs){
        return lhs.second.second < rhs.second.second;
    });

    std::cout << "Guard Id: " << it2->first << std::endl;
    std::cout << "Minute: " << it2->second.first << std::endl;
    std::cout << "Times: " << it2->second.second << std::endl;
    std::cout << "Part2: " << it2->first * it2->second.first << std::endl;
}

int main() {
    auto const input_values = read_input("input");
    // std::for_each(std::cbegin(input_values), std::cend(input_values), [](auto const& elem) {
    //     std::cout << "[" 
    //                     << (elem.first.tm_year + 1900) << "-" 
    //                     << std::setfill('0') << std::setw(2) << (elem.first.tm_mon + 1) << "-" 
    //                     << std::setfill('0') << std::setw(2) << elem.first.tm_mday << " "
    //                     << std::setfill('0') << std::setw(2) << elem.first.tm_hour << ":"
    //                     << std::setfill('0') << std::setw(2) << elem.first.tm_min << 
    //                  "] -> "
    //               << static_cast<int>(elem.second.first) << "," << elem.second.second << std::endl;
    // });

    part1(input_values);
    part2(input_values);

    return 0;
}