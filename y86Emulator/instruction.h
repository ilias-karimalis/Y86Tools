#ifndef Y86EMULATOR_INSTRUCTION_H
#define Y86EMULATOR_INSTRUCTION_H

#include <optional>
#include <ostream>

#include "opcode.h"

enum class Condition {
    NoComparison,
    LessEqual,
    Less,
    Equal,
    NotEqual,
    GreaterEqual,
    Greater
};

constexpr const char* conditionString(Condition condition);

enum class Operation {
    ADD, SUB, AND, XOR,
    MUL, DIV, MOD
};

enum class Register {
    RAX, RCX, RDX, RBX,
    RSP, RBP, RSI, RDI,
    R8,  R9,  R10, R11,
    R12, R13, R14, RNONE
};

constexpr const char* registerString(Register reg);

constexpr const char* operationString(Operation operation);

struct Instruction {

    OpCode opcode;
    Condition func_code;
    std::optional<Register> reg_a;
    std::optional<Register> reg_b;
    std::optional<uint64_t> val_c;
    uint64_t location;

    friend std::ostream& operator<<(std::ostream& stream, Instruction& instruction);



};


#endif //Y86EMULATOR_INSTRUCTION_H
