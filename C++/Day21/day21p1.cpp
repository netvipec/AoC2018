#include <bits/stdc++.h>

typedef std::array<int64_t, 6> registers_t;

struct instruction {
    std::string opcode;
    int64_t input1;
    int64_t input2;
    int64_t output;

    instruction(std::string op, int64_t i1, int64_t i2, int64_t o) : opcode(op), input1(i1), input2(i2), output(o) {}

    friend std::ostream& operator<<(std::ostream& os, const instruction& inst);
};

std::ostream& operator<<(std::ostream& os, const instruction& inst) {
    os << inst.opcode << " " << inst.input1 << " " << inst.input2 << " " << inst.output;
    return os;
}

typedef std::pair<int, std::vector<instruction>> vec_input;

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
typedef std::map<std::string, opcodes_func_t> opcodes_t;

opcodes_t opcodes {
    { "addr", &addr },
    { "addi", &addi },
    { "mulr", &mulr },
    { "muli", &muli },
    { "banr", &banr },
    { "bani", &bani },
    { "borr", &borr },
    { "bori", &bori },
    { "setr", &setr },
    { "seti", &seti },
    { "gtir", &gtir },
    { "gtri", &gtri },
    { "gtrr", &gtrr },
    { "eqir", &eqir },
    { "eqri", &eqri },
    { "eqrr", &eqrr }
};

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

    std::string ip;
    std::string inst;

    std::getline(infile, ip);
    auto const ip_part = split(ip, ' ');
    assert(ip_part.size() == 2);
    input_values.first = std::stoi(ip_part[1]);
    // std::cout << "#ip " << input_values.first << std::endl;

    while (std::getline(infile, inst)) {
        auto const inst_part = split(inst, ' ');
        assert(inst_part.size() == 4);

        std::string const op = inst_part[0];
        int const in1 = std::stoi(inst_part[1]);
        int const in2 = std::stoi(inst_part[2]);
        int const out = std::stoi(inst_part[3]);
        input_values.second.emplace_back(op, in1, in2, out);

        // std::cout << input_values.second.back() << std::endl;
    }
    
    return input_values;
}

int execute_times(int interesting_inst, vec_input const& input_values) {
    registers_t regs{};
    int ip = 0;
    int counter = 0;
    while (ip < static_cast<int>(input_values.second.size())) {
        regs[input_values.first] = ip;

        auto const& actual_inst = input_values.second[ip];

        if (ip == interesting_inst) {
            if (actual_inst.opcode == "eqrr") {
                return (actual_inst.input1 == 0) ? regs[actual_inst.input2] : regs[actual_inst.input1];
            } else if (actual_inst.opcode == "eqir") {
                return actual_inst.input1;
            } else if (actual_inst.opcode == "eqri") {
                return actual_inst.input2;
            }
        }

        auto inst_it = opcodes.find(actual_inst.opcode);
        assert(inst_it != std::cend(opcodes));

        inst_it->second(regs, actual_inst);
        ip = regs[input_values.first];
        ++ip;
        ++counter;        
    }

    return -1;
}

void part1(vec_input const& input_values) {
    auto const interesting_inst = std::find_if(std::cbegin(input_values.second), std::cend(input_values.second), [](auto const& inst){
        return (inst.opcode == "eqrr" && (inst.input1 == 0 || inst.input2 == 0))
            || (inst.opcode == "eqir" && inst.input2 == 0)
            || (inst.opcode == "eqri" && inst.input1 == 0);
    });

    auto const interesting_inst_index = std::distance(std::cbegin(input_values.second), interesting_inst);
    auto const reg0_value = execute_times(interesting_inst_index, input_values);
    std::cout << "Part1: " << reg0_value << std::endl;
}

void part2([[maybe_unused]] vec_input const& input_values) {
    // registers_t regs{};
    // regs[3] = 12691260;
    // int ip = 0;
    // int counter = 0;
    // while (ip < static_cast<int>(input_values.second.size())) {
    //     regs[input_values.first] = ip;

    //     auto const& actual_inst = input_values.second[ip];
    //     auto inst_it = opcodes.find(actual_inst.opcode);
    //     assert(inst_it != std::cend(opcodes));

    //     inst_it->second(regs, actual_inst);
    //     ip = regs[input_values.first];
    //     ++ip;
    //     ++counter;

    //     if (ip == 28) {
    //         std::cout << "Number: " << regs[3] << std::endl;
    //     }
    // }

    std::unordered_set<int64_t> visible;
    constexpr int64_t original_seed = 0x54ced6;
    int64_t seed = original_seed;
    int64_t other = 0x10000;
    int64_t last_inserted = 0;
    for (;;) {
        seed += (other & 0xff);
        other >>= 8;
        auto next = (seed * 0x1016b) & 0xffffff;
        seed = next;
        if (other == 0) {
            other = next | 0x10000;
            seed = original_seed;

            auto res = visible.insert(next);
            if (!res.second) {
                break;
            }
            last_inserted = next;
        }
    }

    // std::cout << "Visited: " << visible.size() << std::endl;
    std::cout << "Part2: " << last_inserted << std::endl;
}

int main() {
    auto const input_values = read_input("input");

    part1(input_values);
    part2(input_values);

    return 0;
}