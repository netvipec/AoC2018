#include <bits/stdc++.h>

struct pos {
    int64_t x;
    int64_t y;
    int64_t z;

    pos(int64_t xx, int64_t yy, int64_t zz) : x(xx), y(yy), z(zz) {}

    friend std::ostream& operator<<(std::ostream& os, pos const& p);
    bool operator==(pos const& o) const {
        return std::tie(x, y, z) == std::tie(o.x, o.y, o.z);
    }
    bool operator<(pos const& o) const {
        return std::tie(x, y, z) < std::tie(o.x, o.y, o.z);
    }
};

std::ostream& operator<<(std::ostream& os, pos const& p) {
    os << "<" << p.x << "," << p.y << "," << p.z << ">";
    return os;
}

typedef std::pair<pos, int64_t> nanobot_t;
typedef std::vector<nanobot_t> vec_input;

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

    // pos=<63349547,43955160,3169314>, r=63428872
    // pos=<124672786,46423329,60063431>, r=76633744

    std::string line;

    while (std::getline(infile, line)) {
        auto const line_part = split(line, ' ');
        assert(line_part.size() == 2);

        auto const pos_str =
            line_part[0].substr(5, line_part[0].size() - 5 - 2);
        auto const pos_part = split(pos_str, ',');
        assert(pos_part.size() == 3);

        auto const x = std::stoi(pos_part[0]);
        auto const y = std::stoi(pos_part[1]);
        auto const z = std::stoi(pos_part[2]);

        auto const r_str = line_part[1].substr(2);
        auto const r = std::stoi(r_str);

        input_values.emplace_back(pos{x, y, z}, r);
    }

    return input_values;
}

int64_t manhattan_distance(nanobot_t const& lhs, nanobot_t const& rhs) {
    return std::abs(lhs.first.x - rhs.first.x) +
           std::abs(lhs.first.y - rhs.first.y) +
           std::abs(lhs.first.z - rhs.first.z);
}

std::pair<int, nanobot_t>
part1_helper([[maybe_unused]] vec_input const& input) {
    auto const max_it = std::max_element(std::cbegin(input), std::cend(input),
                                         [](auto const& lhs, auto const& rhs) {
                                             return lhs.second < rhs.second;
                                         });
    assert(max_it != std::cend(input));

    vec_input max_range;
    std::copy_if(
        std::cbegin(input), std::cend(input), std::back_inserter(max_range),
        [&](auto const& elem) { return elem.second == max_it->second; });

    int all_in_range = 0;
    int idx = 0;
    int best_idx = 0;
    std::for_each(
        std::cbegin(max_range), std::cend(max_range),
        [&](auto const& max_range_nanobot) {
            int in_range = 0;
            std::for_each(
                std::cbegin(input), std::cend(input), [&](auto const& nanobot) {
                    auto const calculated_distance =
                        manhattan_distance(max_range_nanobot, nanobot);
                    if (max_range_nanobot.second >= calculated_distance) {
                        ++in_range;
                    }
                });
            // std::cout << "p=" << elem.first << ", r=" << elem.second <<
            // std::endl;
            if (in_range > all_in_range) {
                all_in_range = in_range;
                best_idx = idx;
            }

            idx++;
        });

    return std::make_pair(all_in_range, max_range[best_idx]);
}

void part1([[maybe_unused]] vec_input const& input) {
    auto const best_nanobot = part1_helper(input);

    std::cout << "Part1: " << best_nanobot.first << std::endl;
}

int64_t manhattan_distance(nanobot_t const& lhs, pos const& rhs) {
    return std::abs(lhs.first.x - rhs.x) + std::abs(lhs.first.y - rhs.y) +
           std::abs(lhs.first.z - rhs.z);
}

int64_t manhattan_distance(pos const& lhs) {
    return std::abs(lhs.x) + std::abs(lhs.y) + std::abs(lhs.z);
}

std::vector<pos> get_neighbors(bool three_dimention = true) {
    std::vector<pos> n;
    for (int64_t i = -1; i <= 1; i++) {
        for (int64_t j = -1; j <= 1; j++) {
            for (int64_t k = -1; k <= 1; k++) {
                if (i == 0 && j == 0 && k == 0) {
                    continue;
                }
                n.emplace_back(pos{i, j, k});
            }
        }
    }
    return n;
}

auto const neighbors_3d = get_neighbors();

void part2([[maybe_unused]] vec_input const& input) {
    auto best_pos = std::accumulate(
        std::cbegin(input), std::cend(input), pos(0, 0, 0),
        [&](auto const& base, auto const& elem) {
            return pos(base.x + elem.first.x, base.y + elem.first.y,
                       base.z + elem.first.z);
        });

    best_pos.x /= input.size();
    best_pos.y /= input.size();
    best_pos.z /= input.size();

    auto best_count = std::count_if(
        std::cbegin(input), std::cend(input), [&](auto const& elem) {
            return manhattan_distance(elem, best_pos) < elem.second;
        });

    std::set<std::pair<pos, int64_t>> candidates;
    std::set<std::pair<pos, int64_t>> n_candidates;
    candidates.emplace(best_pos, best_count);

    std::set<pos> visit;
    visit.insert(best_pos);

    // std::cout << "new: " << best_pos << ", count: " << best_count <<
    // std::endl;

    int64_t power = 1e5;
    while (power > 0) {
        // std::cout << "Stride: " << power << std::endl;
        while (candidates.size()) {
            std::for_each(
                std::cbegin(candidates), std::cend(candidates),
                [&](auto const& candidate) {
                    if (candidate.second < best_count) {
                        return;
                    }
                    std::for_each(
                        std::cbegin(neighbors_3d), std::cend(neighbors_3d),
                        [&](auto const& movement) {
                            pos const n_candidate(
                                candidate.first.x + power * movement.x,
                                candidate.first.y + power * movement.y,
                                candidate.first.z + power * movement.z);

                            auto const it = visit.find(n_candidate);
                            if (it != std::cend(visit)) {
                                return;
                            }

                            auto count = std::count_if(
                                std::cbegin(input), std::cend(input),
                                [&](auto const& nanobot) {
                                    return manhattan_distance(nanobot,
                                                              n_candidate) <=
                                           nanobot.second;
                                });

                            // std::cout << "new: " << n_candidate
                            //           << ", count: " << count << std::endl;
                            if (count >= best_count) {
                                // if (count > best_count) {
                                //     std::cout << "new best count: " << count
                                //               << std::endl;
                                // }
                                n_candidates.emplace(n_candidate, count);
                                best_count = count;
                            }

                            visit.insert(n_candidate);
                        });
                });

            std::swap(candidates, n_candidates);
            if (candidates.size()) {
                n_candidates.clear();
            }
        }

        if (power < 1e2) {
            power /= 10;
        } else {
            power /= 1e2;
        }

        auto const sol = std::max_element(
            std::cbegin(n_candidates), std::cend(n_candidates),
            [](auto const& lhs, auto const& rhs) {
                auto const md_lhs = manhattan_distance(lhs.first);
                auto const md_rhs = manhattan_distance(rhs.first);
                return std::tie(lhs.second, md_rhs) <
                       std::tie(rhs.second, md_lhs);
            });

        if (power > 0) {
            n_candidates.clear();
            candidates.emplace(sol->first, sol->second);
            best_count = sol->second;
        }
    }

    auto const sol = std::min_element(
        std::cbegin(n_candidates), std::cend(n_candidates),
        [](auto const& lhs, auto const& rhs) {
            auto const md_lhs = manhattan_distance(lhs.first);
            auto const md_rhs = manhattan_distance(rhs.first);
            return std::tie(rhs.second, md_lhs) < std::tie(lhs.second, md_rhs);
        });

    // std::for_each(std::cbegin(n_candidates), std::cend(n_candidates),
    //               [](auto const& elem) {
    //                   std::cout << elem.first << "," << elem.second
    //                             << std::endl;
    //               });
    // std::cout << "Part2: " << sol->first << std::endl;
    std::cout << "Part2: " << manhattan_distance(sol->first) << std::endl;
}

int main() {
    auto const input_values = read_input("input");

    part1(input_values);
    part2(input_values);

    return 0;
}