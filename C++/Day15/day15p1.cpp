#include <bits/stdc++.h>

#define VERBOSE

struct pairhash {
public:
    template <typename T, typename U>
    std::size_t operator()(const std::pair<T, U>& x) const {
        return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
    }
};

// up, left, right, down
constexpr std::array<std::pair<int, int>, 4> D = {
    std::make_pair(-1, 0), std::make_pair(0, -1), std::make_pair(0, 1),
    std::make_pair(1, 0)};

typedef std::vector<std::pair<std::pair<int, int>, char const&>> neighbors_t;
typedef std::vector<char> vec_char;
typedef std::vector<vec_char> vec_input;

struct entity {
    static int _id;
    int x;
    int y;
    char type;
    int hit_points;
    int id;

    entity(int xx, int yy, char t, int hp, int idid)
        : x(xx), y(yy), type(t), hit_points(hp), id(idid) {}
    entity(int xx, int yy, char t)
        : x(xx), y(yy), type(t), hit_points(200), id(_id++) {}

    bool operator<(entity const& other) const {
        return std::tie(y, x, type) < std::tie(other.y, other.x, other.type);
    }

    bool operator==(entity const& other) const {
        return std::tie(y, x, type, hit_points, id) ==
               std::tie(other.y, other.x, other.type, other.hit_points,
                        other.id);
    }

    neighbors_t neighbors(vec_input const& map) const {
        neighbors_t n;
        std::for_each(
            std::cbegin(D), std::cend(D), [&n, this, &map](auto const& d) {
                n.emplace_back(std::make_pair(x + d.second, y + d.first),
                               map[y + d.first][x + d.second]);
            });
        return n;
    }

    bool alive() const { return hit_points > 0; }

    bool attack() {
        hit_points -= 3;
        return hit_points <= 0;
    }
};

int entity::_id = 0;

typedef std::vector<entity> vec_pos;

vec_input read_input(std::string const& file_path) {
    vec_input input_values;

    std::ifstream infile(file_path);

    std::string line;
    while (std::getline(infile, line)) {
        input_values.emplace_back(
            std::vector<char>(std::cbegin(line), std::cend(line)));
    }

    return input_values;
}

vec_pos get_pos(vec_input const& map, char goblins, char elves) {
    vec_pos pos;
    int y = 0;
    std::for_each(std::cbegin(map), std::cend(map),
                  [&y, &pos, goblins, elves](auto const& row) {
                      int x = 0;
                      std::for_each(
                          std::cbegin(row), std::cend(row),
                          [&x, &y, &pos, goblins, elves](auto const& elem) {
                              if (elem == goblins || elem == elves) {
                                  pos.emplace_back(x, y, elem);
                              }
                              ++x;
                          });
                      ++y;
                  });
    return pos;
}

entity bfs(entity const& origin, vec_input const& map,
           vec_pos const& entities) {
    std::vector<std::vector<int>> map_back(
        map.size(), std::vector<int>(map.front().size(), -1));

    // Find enemies
    std::set<entity> places{origin};
    std::set<entity> new_places;
    std::set<entity> end_places;
    int steps = 0;
    map_back[origin.y][origin.x] = steps;
    while (!places.empty()) {
        steps++;
        bool found_enemy = false;
        std::for_each(
            std::cbegin(places), std::cend(places), [&](auto const& p) {
                auto const neighbors = p.neighbors(map);
                std::for_each(
                    std::cbegin(neighbors), std::cend(neighbors),
                    [&](auto const& n) {
                        if (n.second == '#') {
                            return;
                        }
                        entity en(n.first.first, n.first.second, n.second,
                                  p.hit_points, p.id);
                        if (n.second == '.') {
                            if (map_back[n.first.second][n.first.first] == -1) {
                                new_places.insert(en);
                                map_back[en.y][en.x] = steps;
                            }
                            return;
                        }
                        auto const enemy_it = std::find_if(
                            std::cbegin(entities), std::cend(entities),
                            [&](auto const& e) {
                                return e.alive() && en.x == e.x &&
                                       en.y == e.y &&
                                       std::abs(origin.type - e.type) == 2;
                            });

                        if (enemy_it != std::cend(entities)) {
                            en.type = enemy_it->type;
                            found_enemy = true;
                            new_places.insert(en);
                            map_back[en.y][en.x] = steps;
                            end_places.insert(p);
                            return;
                        }
                    });
            });

        new_places.swap(places);
        new_places.clear();

        if (found_enemy) {
            break;
        }
    }

    places.clear();

    if (end_places.empty()) {
        return origin;
    }

    auto const best_place = end_places.cbegin();

    // Back track
    places.insert(*best_place);
    steps = map_back[best_place->y][best_place->x];
    if (steps == 0) {
        return origin;
    }
    for (;;) {
        steps--;
        if (steps == 0) {
            break;
        }

        std::for_each(
            std::cbegin(places), std::cend(places), [&](auto const& p) {
                auto const neighbors = p.neighbors(map);
                std::for_each(
                    std::cbegin(neighbors), std::cend(neighbors),
                    [&](auto const& n) {
                        if (map_back[n.first.second][n.first.first] == steps) {
                            new_places.insert(entity(n.first.first,
                                                     n.first.second, n.second));
                        }
                    });
            });

        new_places.swap(places);
        new_places.clear();
    }

    auto new_place = origin;
    new_place.x = places.cbegin()->x;
    new_place.y = places.cbegin()->y;
    return new_place;
}

void print(vec_input map, vec_pos const& entities) {
    std::for_each(std::cbegin(entities), std::cend(entities),
                  [&](auto const& e) { map[e.y][e.x] = e.type; });
    int y = 0;
    std::for_each(std::cbegin(map), std::cend(map), [&](auto const& row) {
        std::for_each(std::cbegin(row), std::cend(row),
                      [&](auto const& cell) { std::cout << cell; });
        auto it = std::find_if(std::cbegin(entities), std::cend(entities),
                               [=](auto const& elem) { return elem.y == y; });
        if (it != std::cend(entities)) {
            std::cout << "   ";
            while (it != std::cend(entities) && it->y == y) {
                std::cout << it->type << "(" << it->hit_points << "), ";
                it++;
            }
        }
        std::cout << std::endl;
        y++;
    });
}

void part1(vec_input input_values) {
    auto entities = get_pos(input_values, 'G', 'E');

    int round = 0;
    for (;; round++) {
        std::sort(std::begin(entities), std::end(entities));

#ifdef VERBOSE
        std::cout << "Round: " << round << std::endl;
        print(input_values, entities);
#endif

        for (auto& elem : entities) {
            if (!elem.alive()) {
                continue;
            }

            auto count_enemies = std::count_if(
                std::cbegin(entities), std::cend(entities), [&](auto const& e) {
                    return e.alive() && std::abs(elem.type - e.type) == 2;
                });
            if (count_enemies == 0) {
#ifdef VERBOSE
                std::cout << "Final Round: " << round << std::endl;
                print(input_values, entities);
#endif

                auto const total_hit_points = std::accumulate(
                    std::cbegin(entities), std::cend(entities), 0ll,
                    [](auto const& base, auto const& elem) {
                        return elem.alive() ? (base + elem.hit_points) : base;
                    });
                std::cout << "Rounds: " << round << std::endl;
                std::cout << "Total Hit Points: " << total_hit_points
                          << std::endl;
                std::cout << "Part1: " << round * total_hit_points << std::endl;
                return;
            }

            // Verify for move
            auto posible_moves = bfs(elem, input_values, entities);

            auto neighbors = posible_moves.neighbors(input_values);

            // Verify for attack
            std::vector<entity*> enemies;
            std::for_each(std::cbegin(neighbors), std::cend(neighbors),
                          [&](auto const& n) {
                              auto const entity_it = std::find_if(
                                  std::begin(entities), std::end(entities),
                                  [&](auto const& e) {
                                      return e.alive() &&
                                             std::abs(e.type - elem.type) ==
                                                 2 &&
                                             e.x == n.first.first &&
                                             e.y == n.first.second;
                                  });
                              if (entity_it == std::end(entities)) {
                                  return;
                              }
                              enemies.push_back(&*entity_it);
                          });

            if (!enemies.empty()) {
                std::sort(std::begin(enemies), std::end(enemies),
                          [](auto const& lhs, auto const& rhs) {
                              return std::tie(lhs->hit_points, *lhs) <
                                     std::tie(rhs->hit_points, *rhs);
                          });
                auto enemy = enemies.front();
                enemy->attack();
                if (!enemy->alive()) {
                    input_values[enemy->y][enemy->x] = '.';
                }
            }
            if (elem.x != posible_moves.x || elem.y != posible_moves.y) {
                input_values[elem.y][elem.x] = '.';
                input_values[posible_moves.y][posible_moves.x] =
                    posible_moves.type;
            }
            elem = posible_moves;
        }

        // Remove dead entities
        auto last = std::remove_if(std::begin(entities), std::end(entities),
                                   [](auto& elem) { return !elem.alive(); });
        entities.erase(last, entities.end());
    }
}

void part2(vec_input const& input_values) {}

int main() {
    auto const input_values = read_input("input");
    // std::cout << "Map: " << std::endl;
    // std::for_each(std::cbegin(input_values), std::cend(input_values), [](auto
    // const& elem){
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