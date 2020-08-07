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

void part1(vec_input const& input_values) {
    registers_t regs{};
    int ip = 0;
    while (ip < static_cast<int>(input_values.second.size())) {
        regs[input_values.first] = ip;
        
        auto const& actual_inst = input_values.second[ip];
        auto inst_it = opcodes.find(actual_inst.opcode);
        assert(inst_it != std::cend(opcodes));

        inst_it->second(regs, actual_inst);
        ip = regs[input_values.first];
        ++ip;
    }

    std::cout << "Part1: " << regs[0] << std::endl;
}

void part2(vec_input const& input_values) {
    // registers_t regs{};
    // regs[0] = 1;
    // int ip = regs[input_values.first];
    // int64_t counter = 0;
    // int64_t last_regs0 = regs[0];
    // while (ip < static_cast<int>(input_values.second.size())) {
    //     regs[input_values.first] = ip;
        
    //     auto const& actual_inst = input_values.second[ip];
    //     auto inst_it = opcodes.find(actual_inst.opcode);
    //     assert(inst_it != std::cend(opcodes));

    //     inst_it->second(regs, actual_inst);
    //     ip = regs[input_values.first];
    //     ++ip;
    //     ++counter;

    //     if (regs[0] != last_regs0) {
    //         last_regs0 = regs[0];
    //         int c = 0;
    //         std::cout << std::setw(16) << std::setfill(' ') << std::right << counter << " -> regs ";
    //         std::for_each(std::cbegin(regs), std::cend(regs), [&](auto const& elem){
    //             std::cout << c++ << "=" << std::setw(10) << std::setfill(' ') << std::left << elem << ",";
    //         });
    //         std::cout << std::endl;
    //     }
    // }

    // Regs[5] -> contain the target number
    // Regs[3] -> contain a divisor of the target number
    // Regs[0] -> contain the sums of the divisors of the target number

    registers_t regs{};
    regs[0] = 1;
    int ip = regs[input_values.first];
    int64_t counter = 0;
    int64_t last_regs0 = regs[0];
    while (ip < static_cast<int>(input_values.second.size())) {
        regs[input_values.first] = ip;
        
        auto const& actual_inst = input_values.second[ip];
        auto inst_it = opcodes.find(actual_inst.opcode);
        assert(inst_it != std::cend(opcodes));

        inst_it->second(regs, actual_inst);
        ip = regs[input_values.first];
        ++ip;
        ++counter;

        if (regs[0] != last_regs0) {
            break;
        }
    }

    auto const target_number = regs[5];
    int64_t divisors_sum = 1;
    for (int i = 2; i <= target_number; ++i) {
        if (target_number % i == 0) {
            divisors_sum += i;
        }
    }

    std::cout << "Part2: " << divisors_sum << std::endl;
}

int main() {
    auto const input_values = read_input("input");

    part1(input_values);
    part2(input_values);

    return 0;
}