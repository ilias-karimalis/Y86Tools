#include "instruction.h"


std::ostream &operator<<(std::ostream &stream, Instruction& instruction) {

    // OpCode::CODE, {Condition/Operation, [based on OpCode]} rA: rB: valC: len:

    // Instruction codes
    stream << opcodeString(instruction.opcode) << ", ";

    // Condition/Operation codes
    switch (instruction.opcode) {

        case OpCode::HALT:
        case OpCode::NOP:
        case OpCode::IRMOVQ:
        case OpCode::RMMOVQ:
        case OpCode::MRMOVQ:
        case OpCode::CALL:
        case OpCode::RET:
        case OpCode::PUSH:
        case OpCode::POP:
            break;

        case OpCode::RRMVXX:
        case OpCode::JUMP:
            stream << "Condition: " << conditionString(instruction.func_code) << ", ";
            break;

        case OpCode::OPQ:
            stream << "Operation: " << operationString(static_cast<Operation>(instruction.func_code)) << ", ";
            break;
    }

    // Registers and valC
    switch (instruction.opcode) {

        case OpCode::HALT:
        case OpCode::NOP:
        case OpCode::RET:
            break;

        case OpCode::RRMVXX:
        case OpCode::OPQ:
            stream << "rA: " << registerString(*instruction.reg_a) << " ";
            stream << "rB: " << registerString(*instruction.reg_b) << ", ";
            break;
        case OpCode::IRMOVQ:
            stream << "rB: " << registerString(*instruction.reg_b) << ", ";
            stream << "valC: " << *instruction.val_c << ", ";
            break;
        case OpCode::RMMOVQ:
        case OpCode::MRMOVQ:
            stream << "rA: " << registerString(*instruction.reg_a) << " ";
            stream << "rB: " << registerString(*instruction.reg_b) << ", ";
            stream << "valC: " << *instruction.val_c << ", ";
            break;
        case OpCode::JUMP:
        case OpCode::CALL:
            stream << "valC: " << *instruction.val_c << ", ";
            break;
        case OpCode::PUSH:
        case OpCode::POP:
            stream << "rA: " << registerString(*instruction.reg_a) << ", ";
            break;
    }

    // Length
    stream << "len: " << instructionLength(instruction.opcode);
    return stream;
}

constexpr const char *conditionString(Condition condition) {
    switch (condition) {

        case Condition::NoComparison:
            return "No Comparison";
        case Condition::LessEqual:
            return "<= 0";
        case Condition::Less:
            return "< 0";
        case Condition::Equal:
            return "== 0";
        case Condition::NotEqual:
            return "!= 0";
        case Condition::GreaterEqual:
            return ">= 0";
        case Condition::Greater:
            return "> 0";
    }
}

constexpr const char *operationString(Operation operation) {
    switch (operation) {

        case Operation::ADD:
            return "ADD +";
        case Operation::SUB:
            return "SUB -";
        case Operation::AND:
            return "AND &";
        case Operation::XOR:
            return "XOR ^";
        case Operation::MUL:
            return "MUL *";
        case Operation::DIV:
            return "DIV /";
        case Operation::MOD:
            return "MOD %";
    }
}

constexpr const char *registerString(Register reg) {
    switch (reg) {

        case Register::RAX:
            return "%rax";
        case Register::RCX:
            return "%rcx";
        case Register::RDX:
            return "%rdx";
        case Register::RBX:
            return "%rbx";
        case Register::RSP:
            return "%rsp";
        case Register::RBP:
            return "%rbp";
        case Register::RSI:
            return "%rsi";
        case Register::RDI:
            return "%rdi";
        case Register::R8:
            return "%r8";
        case Register::R9:
            return "%r9";
        case Register::R10:
            return "%r10";
        case Register::R11:
            return "%r11";
        case Register::R12:
            return "%r12";
        case Register::R13:
            return "%r13";
        case Register::R14:
            return "%r14";
        case Register::RNONE:
            return "%rNONE";
    }
}
