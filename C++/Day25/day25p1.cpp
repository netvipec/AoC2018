#include <bits/stdc++.h>

using ll = int64_t;
using result1_t = ll;
using result2_t = ll;

struct point {
    ll x;
    ll y;
    ll z;
    ll w;

    point(ll xx, ll yy, ll zz, ll ww) : x(xx), y(yy), z(zz), w(ww) {}

    bool operator<(point const& o) const {
        return std::tie(x, y, z, w) < std::tie(o.x, o.y, o.z, o.w);
    }

    friend std::ostream& operator<<(std::ostream& os, point const& p) {
        os << p.x << "," << p.y << "," << p.z << "," << p.w;
        return os;
    }

    ll manhattan_distance(point const& p) const {
        return std::abs(x - p.x) + std::abs(y - p.y) + std::abs(z - p.z) +
               std::abs(w - p.w);
    }
};

using input_t = std::vector<point>;

static std::vector<std::string> split(std::string const& s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

input_t read_input() {
    input_t input_values;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }
        auto const fixed_points = split(line, ',');
        auto const x = std::stoi(fixed_points[0]);
        auto const y = std::stoi(fixed_points[1]);
        auto const z = std::stoi(fixed_points[2]);
        auto const w = std::stoi(fixed_points[3]);
        input_values.push_back(point(x, y, z, w));
    }
    return input_values;
}

result1_t solve1(input_t const& input_data) {
    std::vector<std::vector<ll>> matrix_md(input_data.size(),
                                           std::vector<ll>(input_data.size()));

    std::vector<std::set<ll>> constellations_data;
    std::vector<std::set<ll>*> constellations;
    constellations_data.reserve(input_data.size());
    constellations.reserve(input_data.size());
    for (int i = 0; i < static_cast<int>(input_data.size()); i++) {
        constellations_data.push_back(std::set<ll>{i});
        constellations.push_back(&constellations_data.back());
    }

    for (int i = 0; i < static_cast<int>(input_data.size()) - 1; i++) {
        for (int j = i + 1; j < static_cast<int>(input_data.size()); j++) {
            auto const md = input_data[i].manhattan_distance(input_data[j]);
            if (md <= 3 && constellations[i] != constellations[j]) {
                auto c = constellations[j];

                constellations[i]->insert(c->begin(), c->end());
                std::for_each(std::cbegin(*c), std::cend(*c),
                              [&](auto const& elem) {
                                  constellations[elem] = constellations[i];
                              });
                c->clear();
            }
        }
    }

    std::sort(std::begin(constellations), std::end(constellations));
    auto last =
        std::unique(std::begin(constellations), std::end(constellations));
    // v now holds {1 2 1 3 4 5 4 x x x}, where 'x' is indeterminate
    constellations.erase(last, constellations.end());
    return constellations.size();
}

result2_t solve2(input_t const& input_data) {
    result2_t ans = -1;
    return ans;
}

int main() {
    auto begin_input = std::chrono::steady_clock::now();
    auto const input_data = read_input();
    auto end_input = std::chrono::steady_clock::now();
    // std::for_each(std::cbegin(input_data), std::cend(input_data),
    //               [](auto const& elem) { std::cout << elem << std::endl; });
    // std::cout << std::endl;
    std::cout << "Read Input Time = "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     end_input - begin_input)
                     .count()
              << "[ms]" << std::endl;

    auto begin_s1 = std::chrono::steady_clock::now();
    auto const s1 = solve1(input_data);
    auto end_s1 = std::chrono::steady_clock::now();
    std::cout << "Solution Part1: " << s1 << "   -> Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end_s1 -
                                                                       begin_s1)
                     .count()
              << "[ms]" << std::endl;

    auto begin_s2 = std::chrono::steady_clock::now();
    auto const s2 = solve2(input_data);
    auto end_s2 = std::chrono::steady_clock::now();
    std::cout << "Solution Part2: " << s2 << "   -> Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end_s2 -
                                                                       begin_s2)
                     .count()
              << "[ms]" << std::endl;

    return 0;
}