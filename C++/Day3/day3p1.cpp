#include <bits/stdc++.h>

typedef std::vector<std::pair<std::pair<int, int>, std::pair<int, int>>> vec_input;
typedef std::vector<int> vec_int;

// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
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

vec_input read_input(std::string const& file_path) {
    vec_input input_values;

    std::ifstream infile(file_path);

    std::string line;
    while (std::getline(infile, line)) {
        auto const s = split(line, '@');
        assert(s.size() == 2);

        auto const ss = split(s[1], ':');
        assert(ss.size() == 2);

        auto p = split(ss[0], ',');
        std::pair<int, int> p1;
        trim(p[0]);
        p1.first  = std::atoi(p[0].c_str());
        trim(p[1]);
        p1.second = std::atoi(p[1].c_str());

        auto w = split(ss[1], 'x');
        std::pair<int, int> w1;
        trim(w[0]);
        w1.first  = std::atoi(w[0].c_str());
        trim(w[1]);
        w1.second = std::atoi(w[1].c_str());
        input_values.emplace_back(p1, w1);
    }
    return input_values;
}

// Only for pairs of std::hash-able types for simplicity.
// You can of course template this struct to allow other hash functions
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        // Mainly for demonstration purposes, i.e. works but is overly simple
        // In the real world, use sth. like boost.hash_combine
        return h1 ^ h2;  
    }
};

void part1(vec_input const& input_values) {
    std::unordered_map<std::pair<int, int>, int, pair_hash> overlap;

    std::for_each(std::cbegin(input_values), std::cend(input_values), [&](auto const& elem){
        for (int i = 0; i < elem.second.first; ++i) {
            for (int j = 0; j < elem.second.second; ++j) {
                ++overlap[std::make_pair(elem.first.first + i, elem.first.second + j)];
            }    
        }
    });

    auto const count = std::count_if(std::cbegin(overlap), std::cend(overlap), [](auto const& elem){
        return elem.second > 1;
    });

    std::cout << "Part1: " << count << std::endl;
}

void part2(vec_input const& input_values) {
    std::unordered_map<std::pair<int, int>, std::unordered_set<int>, pair_hash> overlap;
    std::vector<bool> not_overlap_id(input_values.size(), true);

    int id = 0;
    std::for_each(std::cbegin(input_values), std::cend(input_values), [&](auto const& elem){
        for (int i = 0; i < elem.second.first; ++i) {
            for (int j = 0; j < elem.second.second; ++j) {
                auto& v = overlap[std::make_pair(elem.first.first + i, elem.first.second + j)];
                if (v.empty()) {
                    v.insert(id);
                } else {
                    v.insert(id);
                    std::for_each(std::cbegin(v), std::cend(v), [&](auto const& elem){
                        not_overlap_id[elem] = false;
                    });
                }
            }    
        }
        ++id;
    });

    auto c = std::count_if(std::cbegin(not_overlap_id), std::cend(not_overlap_id), [](auto const& elem){
        return elem;
    });

    assert(c == 1);
    auto it = std::find_if(std::cbegin(not_overlap_id), std::cend(not_overlap_id), [](auto const& elem){
        return elem;
    });
    std::cout << "Part2: " << std::distance(not_overlap_id.cbegin(), it) + 1 << std::endl;
}

int main() {
    auto const input_values = read_input("input");
    // std::for_each(std::cbegin(input_values), std::cend(input_values), [](auto const& elem){
    //     std::cout << "[" << elem.first.first << "," << elem.first.second << " -> " << elem.second.first << "," << elem.second.second << "], ";
    // });
    // std::cout << std::endl;

    part1(input_values);
    part2(input_values);

    return 0;
}