#ifndef Y86EMULATOR_PARSER_H
#define Y86EMULATOR_PARSER_H

#include <vector>
#include <array>

#include "token.h"
#include "lexer.h"

struct Parser {

    struct ParseNode {

        enum class Instruction {
            HALT, NOP, RRMOVQ, CMOVLE, CMOVL, CMOVE, CMOVNE, CMOVGE,
            CMOVG, IRMOVQ, RMMOVQ, MRMOVQ, ADDQ, SUBQ, ANDQ, XORQ,
            JMP, JLE, JL, JE, JNE, JGE, JG, CALL, RET, PUSHQ, POPQ
        };

        enum class Type {
            POS,
            ALIGN,
            QUAD,
            LABEL,
            INSTRUCTION,
        };

        enum NameIndices {
            // Registers
            RAX, RCX, RDX, RBX, RSP, RBP, RSI,
            RDI, R8, R9, R10, R11, R12, R13, R14,

            // Instructions
            HALT, NOP, RRMOVQ, CMOVLE, CMOVL, CMOVE, CMOVNE, CMOVGE,
            CMOVG, IRMOVQ, RMMOVQ, MRMOVQ, ADDQ, SUBQ, ANDQ, XORQ,
            JMP, JLE, JL, JE, JNE, JGE, JG, CALL, RET, PUSHQ, POPQ,

            // Directives
            POS, ALIGN, QUAD
        };

        int index;
        int len;
        Type t;
        union {
            uint64_t value;
            Instruction ins;
        } u;
    };

    NameMap name_map;
    std::vector<const char*> names;

    Parser(NameMap name_map);

    std::vector<ParseNode> parse(const std::vector<Token>& tokvec);

    ParseNode parse_immediate(const std::vector<Token>& tokvec, int index);

    ParseNode parse_identifier(const std::vector<Token> &tokvec, int index);

    ParseNode parse_instruction(const std::vector<Token> &tokvec, int i, ParseNode::NameIndices name_index);

    void parse_registers(const std::vector<Token> &vector, int index);

};


#endif //Y86EMULATOR_PARSER_H
