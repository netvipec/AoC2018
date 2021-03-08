#include <bits/stdc++.h>

using ll = int64_t;

#define VERBOSE 0

// trim from start (in place)
static inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    [](int ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](int ch) { return !std::isspace(ch); })
                .base(),
            s.end());
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

class army {
    ll _units = -1;
    ll _hit_points = -1;
    ll _attack_points = -1;
    std::string _attack_type;
    ll _initiative = -1;
    std::set<std::string> _immune;
    std::set<std::string> _weak;
    ll _team_id = -1;
    ll _idx = -1;

public:
    army(std::string const& line, ll team_id, ll idx) {
        read(line, team_id, idx);
    }

    ll effective_power() const { return _units * _attack_points; }
    ll initiative() const { return _initiative; }
    ll hit_points() const { return _hit_points; }
    ll units() const { return _units; }
    ll idx() const { return _idx; }
    ll team_id() const { return _team_id; }
    bool alive() const { return _units > 0; }

    ll attack_points() const { return _attack_points; }
    void change_attack_points(ll diff_attack_points) {
        _attack_points += diff_attack_points;
    }

    ll attacked(army const& other) {
        auto const damage = other.damage(*this);

        auto const units_removed = std::min(damage / _hit_points, _units);
        _units -= units_removed;
#if VERBOSE > 1
        std::cout << "Kill " << units_removed << " units. remaining: " << _units
                  << std::endl;
#endif
        return units_removed;
    }

    void read(std::string const& line, ll team_id, ll idx) {
        _idx = idx;
        _team_id = team_id;

        auto const elems = split(line, ' ');
        _units = std::stoi(elems.front());
        _hit_points = std::stoi(elems[4]);
        auto const idx1 = line.find('(');
        if (idx1 != std::string::npos) {
            auto const idx2 = line.find(')', idx1);
            assert(idx2 != std::string::npos);

            auto const iw = line.substr(idx1 + 1, idx2 - idx1 - 1);
            auto const elem_iw = split(iw, ';');

            std::for_each(
                std::cbegin(elem_iw), std::cend(elem_iw), [&](auto const& e) {
                    auto ee = e;
                    trim(ee);
                    assert(ee.front() == 'i' || ee.front() == 'w');
                    auto& set_to_add = (ee.front() == 'i') ? _immune : _weak;
                    auto const init_size = (ee.front() == 'i') ? 10 : 8;
                    auto const ss = ee.substr(init_size);
                    auto const elem_type = split(ss, ',');
                    std::for_each(std::cbegin(elem_type), std::cend(elem_type),
                                  [&](auto const& et) {
                                      auto ett = et;
                                      trim(ett);
                                      set_to_add.insert(ett);
                                  });
                });
        }
        _attack_points = std::stoi(elems[elems.size() - 1 - 5]);
        _attack_type = elems[elems.size() - 1 - 4];
        _initiative = std::stoi(elems.back());
    }

    friend std::ostream& operator<<(std::ostream& os, army const& a) {
        os << (a._team_id == 0 ? "Immune System" : "Infections") << " group "
           << a._idx + 1 << ": ";
        os << a._units << " units each with " << a._hit_points
           << " hit points ";
        if (!a._weak.empty() || !a._immune.empty()) {
            os << "(";
            if (!a._immune.empty()) {
                os << "immune to ";
                ll idx = 0;
                std::for_each(
                    std::cbegin(a._immune), std::cend(a._immune),
                    [&](auto const& ie) { os << ((idx++) ? ", " : "") << ie; });
            }
            if (!a._weak.empty()) {
                if (!a._immune.empty()) {
                    os << "; ";
                }
                os << "weak to ";
                ll idx = 0;
                std::for_each(
                    std::cbegin(a._weak), std::cend(a._weak),
                    [&](auto const& ie) { os << ((idx++) ? ", " : "") << ie; });
            }
            os << ") ";
        }
        os << "with an attack that does " << a._attack_points << " "
           << a._attack_type << " damage at initiative " << a._initiative;
        return os;
    }

    ll damage(army const& other) const {
        auto const inmmune_it = other._immune.find(_attack_type);
        if (inmmune_it != std::cend(other._immune)) {
            return 0;
        }
        auto const weak_it = other._weak.find(_attack_type);
        return effective_power() * (weak_it == std::cend(other._weak) ? 1 : 2);
    }
};

using vec_army = std::vector<army>;
using input_t = vec_army;
using result1_t = ll;
using result2_t = ll;

void read_army(ll team_id, vec_army& armies) {
    std::string line;
    ll idx = 0;
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            break;
        }

        army a(line, team_id, idx++);
        armies.push_back(a);

        // std::cout << a << std::endl;
    }
}

input_t read_input() {
    input_t input_values;

    std::string line;
    std::getline(std::cin, line);
    assert(line == "Immune System:");
    read_army(0, input_values);

    std::getline(std::cin, line);
    assert(line == "Infection:");
    read_army(1, input_values);
    return input_values;
}

ll select_target(ll attacker_idx, input_t const& input_data,
                 std::vector<ll> const& already_selected) {
    auto const& attacker = input_data[attacker_idx];

    if (!attacker.alive()) {
        return -1;
    }

    std::vector<ll> candidates;
    ll idx = 0;
    std::for_each(std::cbegin(input_data), std::cend(input_data),
                  [&](auto const& elem) {
                      auto it = std::find(std::cbegin(already_selected),
                                          std::cend(already_selected), idx);
                      if (elem.team_id() != attacker.team_id() &&
                          it == std::cend(already_selected) && elem.alive() &&
                          attacker.damage(elem) > 0) {
                          candidates.push_back(idx);
                      }
                      idx++;
                  });

    if (candidates.empty()) {
        return -1;
    }

    std::sort(std::begin(candidates), std::end(candidates),
              [&](auto const& lhs, auto const& rhs) {
                  auto const dam_l = attacker.damage(input_data[lhs]);
                  auto const dam_r = attacker.damage(input_data[rhs]);
                  auto const ep_l = input_data[lhs].effective_power();
                  auto const ep_r = input_data[rhs].effective_power();
                  auto const i_l = input_data[lhs].initiative();
                  auto const i_r = input_data[rhs].initiative();
                  return std::tie(dam_l, ep_l, i_l) >
                         std::tie(dam_r, ep_r, i_r);
              });

    return candidates.front();
}

std::pair<result1_t, bool> solve_helper(input_t input_data) {
    std::vector<ll> attack_order(input_data.size());
    std::iota(std::begin(attack_order), std::end(attack_order), 0);

    std::sort(std::begin(attack_order), std::end(attack_order),
              [&](auto const& lhs, auto const& rhs) {
                  return input_data[lhs].initiative() >
                         input_data[rhs].initiative();
              });

    for (;;) {
        std::vector<ll> target_selection_order(input_data.size());
        std::iota(std::begin(target_selection_order),
                  std::end(target_selection_order), 0);

        std::sort(std::begin(target_selection_order),
                  std::end(target_selection_order),
                  [&](auto const& lhs, auto const& rhs) {
                      auto const ep_l = input_data[lhs].effective_power();
                      auto const ep_r = input_data[rhs].effective_power();
                      auto const i_l = input_data[lhs].initiative();
                      auto const i_r = input_data[rhs].initiative();
                      return std::tie(ep_l, i_l) > std::tie(ep_r, i_r);
                  });

        std::vector<ll> attack_to(input_data.size(), -1);
        std::for_each(std::cbegin(target_selection_order),
                      std::cend(target_selection_order), [&](auto const& elem) {
                          attack_to[elem] =
                              select_target(elem, input_data, attack_to);
                      });

        ll units_removed_in_attack = 0;
        std::for_each(
            std::cbegin(attack_order), std::cend(attack_order),
            [&](auto const& elem) {
                if (attack_to[elem] != -1) {
#if VERBOSE > 1
                    std::cout << "Attacker: " << input_data[elem] << std::endl;
                    std::cout << "Deffender: " << input_data[attack_to[elem]]
                              << std::endl;
#endif

                    units_removed_in_attack +=
                        input_data[attack_to[elem]].attacked(input_data[elem]);
#if VERBOSE > 1
                    std::cout << std::endl;
#endif
                }
            });

        auto units1 = std::accumulate(
            std::cbegin(input_data), std::cend(input_data), 0ll,
            [](auto const& base, auto const& elem) {
                return (elem.team_id() == 0) ? (base + elem.units()) : base;
            });
        auto units2 = std::accumulate(
            std::cbegin(input_data), std::cend(input_data), 0ll,
            [](auto const& base, auto const& elem) {
                return (elem.team_id() == 1) ? (base + elem.units()) : base;
            });

        if (units1 == 0 && units2 == 0) {
            return std::make_pair(-1, false);
        }

        if (units1 == 0 || units2 == 0) {
#if VERBOSE > 0
            // std::for_each(
            //     std::cbegin(input_data), std::cend(input_data),
            //     [](auto const& elem) { std::cout << elem << std::endl; });
#endif
            return (units1 != 0) ? std::make_pair(units1, true)
                                 : std::make_pair(units2, false);
        }

        if (units_removed_in_attack == 0) {
#if VERBOSE > 0
            std::cout << "Finalize by stale mate. Units1: " << units1
                      << ", Units2: " << units2 << std::endl;
#endif
            return std::make_pair(-1, false);
        }

#if VERBOSE > 1
        std::cout << "New round:" << std::endl;
#endif
    }
}

result1_t solve1(input_t const& input_data) {
    return solve_helper(input_data).first;
}

result2_t solve2(input_t const& input_data) {
    result2_t ans = -1;

#if VERBOSE > 0
    std::cout << "Testing boost: 0" << std::endl;
#endif

    auto const result = solve_helper(input_data);
    assert(!result.second);
    ll first_winning_boost = 1;
    ll const step = 2;
    for (;;) {
#if VERBOSE > 0
        std::cout << "Testing boost: " << first_winning_boost << std::endl;
#endif

        auto boost_input_data = input_data;
        std::for_each(std::begin(boost_input_data), std::end(boost_input_data),
                      [=](auto& elem) {
                          if (elem.team_id() == 0) {
                              elem.change_attack_points(first_winning_boost);
                          }
                      });

        auto const result = solve_helper(boost_input_data);
        if (result.second) {
            ans = result.first;
            break;
        }

        first_winning_boost *= step;
    }

#if VERBOSE > 0
    std::cout << "Final: " << first_winning_boost << std::endl;
#endif

    ll hi = first_winning_boost;
    ll lo = first_winning_boost / step;

    while (lo < hi) {
        auto const mi = lo + (hi - lo) / 2;
#if VERBOSE > 0
        std::cout << "Searching in hi: " << hi << ", lo: " << lo
                  << ", mi: " << mi << std::endl;
#endif

        auto boost_input_data = input_data;
        std::for_each(std::begin(boost_input_data), std::end(boost_input_data),
                      [=](auto& elem) {
                          if (elem.team_id() == 0) {
                              elem.change_attack_points(mi);
                          }
                      });

        auto const result = solve_helper(boost_input_data);
        if (result.second) {
            hi = mi;
            ans = result.first;

#if VERBOSE > 0
            std::cout << "New sol: " << ans << std::endl;
#endif
        } else {
            lo = mi + 1;
        }
    }
    return ans;
}

int main() {
    auto begin_input = std::chrono::steady_clock::now();
    auto const input_data = read_input();
    auto end_input = std::chrono::steady_clock::now();
    // std::for_each(std::cbegin(input_data), std::cend(input_data),
    //               [](auto const& elem) { std::cout << elem << std::endl; });
    std::cout << std::endl;
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