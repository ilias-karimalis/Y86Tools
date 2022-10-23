#ifndef Y86EMULATOR_OPCODE_H
#define Y86EMULATOR_OPCODE_H

#include    <cctype>

enum class OpCode {
    HALT,
    NOP,
    RRMVXX,
    IRMOVQ,
    RMMOVQ,
    MRMOVQ,
    OPQ,
    JUMP,
    CALL,
    RET,
    PUSH,
    POP
};

inline constexpr size_t instructionLength(OpCode opcode) {
    switch (opcode) {

        case OpCode::HALT:
        case OpCode::NOP:
        case OpCode::RET:
            return 1;

        case OpCode::RRMVXX:
        case OpCode::OPQ:
        case OpCode::PUSH:
        case OpCode::POP:
            return 2;

        case OpCode::JUMP:
        case OpCode::CALL:
            return 9;

        case OpCode::IRMOVQ:
        case OpCode::RMMOVQ:
        case OpCode::MRMOVQ:
            return 10;
    }
}

inline constexpr const char* opcodeString(OpCode opcode) {
    switch (opcode) {

        case OpCode::HALT:
            return "OpCode::HALT";
        case OpCode::NOP:
            return "OpCode::NOP";
        case OpCode::RRMVXX:
            return "OpCode::RRMVXX";
        case OpCode::IRMOVQ:
            return "OpCode::IRMOVQ";
        case OpCode::RMMOVQ:
            return "OpCode::RMMOVQ";
        case OpCode::MRMOVQ:
            return "OpCode::MRMOVQ";
        case OpCode::OPQ:
            return "OpCode::OPQ";
        case OpCode::JUMP:
            return "OpCode::JUMP";
        case OpCode::CALL:
            return "OpCode::CALL";
        case OpCode::RET:
            return "OpCode::RET";
        case OpCode::PUSH:
            return "OpCode::PUSH";
        case OpCode::POP:
            return "OpCode::POP";
    }
}


#endif //Y86EMULATOR_OPCODE_H
