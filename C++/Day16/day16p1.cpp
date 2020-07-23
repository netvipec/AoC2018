#include <bits/stdc++.h>

typedef std::array<int, 4> registers_t;

struct instruction {
    uint8_t opcode;
    uint8_t input1;
    uint8_t input2;
    uint8_t output;

    instruction(uint8_t op, uint8_t i1, uint8_t i2, uint8_t o) : opcode(op), input1(i1), input2(i2), output(o) {}

    friend std::ostream& operator<<(std::ostream& os, const instruction& inst);
};

std::ostream& operator<<(std::ostream& os, const instruction& inst) {
    os << static_cast<uint32_t>(inst.opcode) << " " << static_cast<uint32_t>(inst.input1) << " "
       << static_cast<uint32_t>(inst.input2) << " " << static_cast<uint32_t>(inst.output);
    return os;
}

struct test_execution {
    registers_t before;
    instruction inst;
    registers_t after;

    test_execution(registers_t b, instruction i, registers_t a) : before(b), inst(i), after(a) {}
};

typedef std::pair<std::vector<test_execution>, std::vector<instruction>> vec_input;

void addr(registers_t& reg, instruction const& inst) { reg[inst.output] = reg[inst.input1] + reg[inst.input2]; }
void addi(registers_t& reg, instruction const& inst) { reg[inst.output] = reg[inst.input1] + inst.input2; }

void mulr(registers_t& reg, instruction const& inst) { reg[inst.output] = reg[inst.input1] * reg[inst.input2]; }
void muli(registers_t& reg, instruction const& inst) { reg[inst.output] = reg[inst.input1] * inst.input2; }

void banr(registers_t& reg, instruction const& inst) { reg[inst.output] = reg[inst.input1] & reg[inst.input2]; }
void bani(registers_t& reg, instruction const& inst) { reg[inst.output] = reg[inst.input1] & inst.input2; }

void borr(registers_t& reg, instruction const& inst) { reg[inst.output] = reg[inst.input1] | reg[inst.input2]; }
void bori(registers_t& reg, instruction const& inst) { reg[inst.output] = reg[inst.input1] | inst.input2; }

void setr(registers_t& reg, instruction const& inst) { reg[inst.output] = reg[inst.input1]; }
void seti(registers_t& reg, instruction const& inst) { reg[inst.output] = inst.input1; }

void gtir(registers_t& reg, instruction const& inst) { reg[inst.output] = ((inst.input1 > reg[inst.input2]) ? 1 : 0); }
void gtri(registers_t& reg, instruction const& inst) { reg[inst.output] = ((reg[inst.input1] > inst.input2) ? 1 : 0); }
void gtrr(registers_t& reg, instruction const& inst) {
    reg[inst.output] = ((reg[inst.input1] > reg[inst.input2]) ? 1 : 0);
}

void eqir(registers_t& reg, instruction const& inst) { reg[inst.output] = ((inst.input1 == reg[inst.input2]) ? 1 : 0); }
void eqri(registers_t& reg, instruction const& inst) { reg[inst.output] = ((reg[inst.input1] == inst.input2) ? 1 : 0); }
void eqrr(registers_t& reg, instruction const& inst) {
    reg[inst.output] = reg[inst.input1] == reg[inst.input2] ? 1 : 0;
}

typedef std::function<void(registers_t&, instruction const&)> opcodes_func_t;
typedef std::vector<opcodes_func_t> opcode_possibilities_t;
typedef std::vector<std::vector<bool>> opcodes_possibilities_t;

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

vec_input read_input(std::string const& file_path) {
    vec_input input_values;

    std::ifstream infile(file_path);

    int op, in1, in2, out;
    std::stringstream stream;

    registers_t reg_b, reg_a;

    std::string before;
    std::string inst;
    std::string after;
    std::string tmp;
    for (;;) {
        std::getline(infile, before);
        if (before.empty()) {
            break;
        }
        std::getline(infile, inst);
        std::getline(infile, after);
        std::getline(infile, tmp);
        assert(tmp.empty());

        std::string bb = before.substr(9, 10);
        auto const bv = split(bb, ',');
        std::transform(std::cbegin(bv), std::cend(bv), reg_b.begin(), [](auto e) {
            trim(e);
            return std::atoi(e.c_str());
        });

        std::string aa = after.substr(9, 10);
        auto const av = split(aa, ',');
        std::transform(std::cbegin(av), std::cend(av), reg_a.begin(), [](auto e) {
            trim(e);
            return std::atoi(e.c_str());
        });

        stream.clear();
        stream.str(inst);
        stream >> op >> in1 >> in2 >> out;

        input_values.first.emplace_back(reg_b, instruction(op, in1, in2, out), reg_a);
    }

    std::getline(infile, tmp);
    assert(tmp.empty());
    while (std::getline(infile, inst)) {
        stream.clear();
        stream.str(inst);
        stream >> op >> in1 >> in2 >> out;

        input_values.second.emplace_back(op, in1, in2, out);
    }

    return input_values;
}

void report_test(test_execution const& test, registers_t const& reg, int opcode_id, bool good) {
    std::cout << (good ? "Valid execution" : "Invalid execution") << std::endl;
    std::cout << "Before: [";
    bool is_first = true;
    std::for_each(std::cbegin(test.before), std::cend(test.before), [&](auto const& e) {
        std::cout << (is_first ? "" : ", ") << e;
        is_first = false;
    });
    std::cout << "]" << std::endl << test.inst << std::endl;
    std::cout << "After:  [";
    is_first = true;
    std::for_each(std::cbegin(test.after), std::cend(test.after), [&](auto const& e) {
        std::cout << (is_first ? "" : ", ") << e;
        is_first = false;
    });
    std::cout << "]" << std::endl;
    std::cout << "After Real:  [";
    is_first = true;
    std::for_each(std::cbegin(reg), std::cend(reg), [&](auto const& e) {
        std::cout << (is_first ? "" : ", ") << e;
        is_first = false;
    });
    std::cout << "]" << std::endl;
    std::cout << " => " << (int)test.inst.opcode << ", " << opcode_id << std::endl << std::endl;
}

void part1(vec_input const& input_values) {
    opcode_possibilities_t opcodes{&addr, &addi, &mulr, &muli, &banr, &bani, &borr, &bori,
                                   &setr, &seti, &gtir, &gtri, &gtrr, &eqir, &eqri, &eqrr};

    int sol = 0;
    std::for_each(std::cbegin(input_values.first), std::cend(input_values.first), [&](auto const& test) {
        int bad = 0;
        for (unsigned int i = 0, i_end = opcodes.size(); i < i_end; ++i) {
            auto reg = test.before;
            opcodes[i](reg, test.inst);
            if (reg != test.after) {
                ++bad;
            }
        }

        sol += ((opcodes.size() - bad) >= 3) ? 1 : 0;
    });

    std::cout << "Part1: " << sol << std::endl;
}

void part2(vec_input const& input_values) {
    opcode_possibilities_t opcodes{&addr, &addi, &mulr, &muli, &banr, &bani, &borr, &bori,
                                   &setr, &seti, &gtir, &gtri, &gtrr, &eqir, &eqri, &eqrr};

    opcodes_possibilities_t opcodes_possibilities(opcodes.size(), std::vector<bool>(opcodes.size(), true));

    std::for_each(std::cbegin(input_values.first), std::cend(input_values.first), [&](auto const& test) {
        for (unsigned int i = 0, i_end = opcodes_possibilities[test.inst.opcode].size(); i < i_end; ++i) {
            auto& actual_opcode = opcodes_possibilities[test.inst.opcode];
            if (actual_opcode[i]) {
                auto reg = test.before;
                opcodes[i](reg, test.inst);
                if (reg != test.after) {
                    actual_opcode[i] = false;
                }

                // report_test(test, reg, i, actual_opcode[i]);
            }
        }
        // std::cout << std::endl;
    });

    std::vector<int> opcodes_index(opcodes.size(), -1);
    for (unsigned int i = 0, i_end = opcodes_index.size(); i < i_end; ++i) {
        auto const op_it = std::find_if(
            std::cbegin(opcodes_possibilities), std::cend(opcodes_possibilities), [&](auto const& func_list) {
                auto const c = std::count(std::cbegin(func_list), std::cend(func_list), true);
                return c == 1;
            });
        assert(op_it != std::cend(opcodes_possibilities));

        auto const op_index = std::distance(std::cbegin(opcodes_possibilities), op_it);

        auto const op_func_it = std::find(std::cbegin(*op_it), std::cend(*op_it), true);
        assert(op_func_it != std::cend(*op_it));

        auto const op_func_index = std::distance(std::cbegin(*op_it), op_func_it);

        opcodes_index[op_index] = op_func_index;

        std::for_each(std::begin(opcodes_possibilities), std::end(opcodes_possibilities),
                      [=](auto& elem) { elem[op_func_index] = false; });
    }

    registers_t reg = {0, 0, 0, 0};
    std::for_each(std::cbegin(input_values.second), std::cend(input_values.second), [&](auto const& inst) {
        auto const actual_index = opcodes_index[inst.opcode];
        opcodes[actual_index](reg, inst);
    });

    std::cout << "Part2: " << reg[0] << std::endl;
}

int main() {
    auto const input_values = read_input("input");
    // std::for_each(std::cbegin(input_values.first), std::cend(input_values.first), [](auto const& elem) {
    //     std::cout << "Before: [";
    //     bool is_first = true;
    //     std::for_each(std::cbegin(elem.before), std::cend(elem.before), [&](auto const& e) {
    //         std::cout << (is_first ? "" : ", ") << e;
    //         is_first = false;
    //     });
    //     std::cout << "]" << std::endl << elem.inst << std::endl;
    //     std::cout << "After:  [";
    //     is_first = true;
    //     std::for_each(std::cbegin(elem.after), std::cend(elem.after), [&](auto const& e) {
    //         std::cout << (is_first ? "" : ", ") << e;
    //         is_first = false;
    //     });
    //     std::cout << "]" << std::endl << std::endl;
    // });
    // std::for_each(std::cbegin(input_values.second), std::cend(input_values.second),
    //               [](auto const& elem) { std::cout << elem << std::endl; });

    part1(input_values);
    part2(input_values);

    return 0;
}