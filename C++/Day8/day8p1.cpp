#include <bits/stdc++.h>

typedef std::vector<int> vec_int;
typedef std::vector<char> vec_char;

class InputRead {
    vec_int::const_iterator m_headers;
    vec_int::const_iterator m_headers_end;

public:
    InputRead(vec_int const& headers)
        : m_headers(std::cbegin(headers)), m_headers_end(std::cend(headers)) {}

    int read() {
        assert(m_headers != m_headers_end);
        auto elem = *m_headers;
        ++m_headers;
        return elem;
    }

    int remaining_size() const {
        return std::distance(m_headers, m_headers_end);
    }
};

template <typename T>
class TreeNode {
    std::vector<TreeNode<T>> m_childrens;
    std::vector<T> m_metadata;
public:
    TreeNode() = default;
    TreeNode(TreeNode const&) = default;
    TreeNode(InputRead& reader);

    template <typename F>
    void visit(F f) const;

    int value() const;

    std::vector<T> const& metadata() const { return m_metadata; }
    std::vector<TreeNode<T>> const& childrens() const { return m_childrens; }
};

template <typename T>
TreeNode<T>::TreeNode(InputRead& reader) {
    assert(reader.remaining_size() >= 2);

    auto const childrens_count = reader.read();
    auto const metadata_count = reader.read();
    std::generate_n(std::back_inserter(m_childrens), childrens_count, [&](){
        return TreeNode(reader);
    });
    std::generate_n(std::back_inserter(m_metadata), metadata_count, [&](){
        return reader.read();
    });
}

template <typename T>
template <typename F>
void TreeNode<T>::visit(F f) const {
    f(*this);
    std::for_each(std::cbegin(m_childrens), std::cend(m_childrens), [&](auto const& elem){
        elem.visit(f);
    });
}

template <typename T>
int TreeNode<T>::value() const {
    if (m_childrens.size() == 0) {
        return std::accumulate(std::cbegin(m_metadata), std::cend(m_metadata), 0);
    } else {
        return std::accumulate(std::cbegin(m_metadata), std::cend(m_metadata), 0, [&](auto const& base, auto const& elem){
            auto const idx = elem - 1;
            if (idx < 0 || idx >= static_cast<int>(m_childrens.size())) {
                return base;
            }
            return base + m_childrens[idx].value();
        });
    }
}

typedef TreeNode<int> vec_input;

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
    std::getline(infile, line);

    auto const values_str = split(line, ' ');
    vec_int values;
    std::transform(std::cbegin(values_str), std::cend(values_str), std::back_inserter(values), [](auto const& elem){
        return std::atoi(elem.c_str());
    });

    InputRead reader(values);

    return vec_input(reader);
}

void part1(vec_input const& input_values) {
    int sum_metadata = 0;
    input_values.visit([&](auto const& node){
        sum_metadata += std::accumulate(std::cbegin(node.metadata()), std::cend(node.metadata()), 0);
    });

    std::cout << "Part1: " << sum_metadata << std::endl;
}

void part2(vec_input const& input_values) {
    auto const root_value = input_values.value();

    std::cout << "Part2: " << root_value << std::endl;
}

int main() {
    auto const input_values = read_input("input");
    // std::for_each(std::cbegin(input_values), std::cend(input_values), [](auto const& elem){
    //     std::cout << elem.first << " -> ";
    //     std::for_each(std::cbegin(elem.second), std::cend(elem.second), [](auto const& elem){
    //         std::cout << elem << ",";
    //     });
    //     std::cout << std::endl;
    // });

    part1(input_values);
    part2(input_values);

    return 0;
}