#include <iostream>
#include "parser.h"
#include "error_handling.h"
#include "log.h"

Parser::Parser(NameMap name_map)
        : name_map(name_map)
{
    // We should add default strings to the NameMap.
    names.resize(ParseNode::NameIndices::QUAD + 1);
    names[ParseNode::RAX] = name_map.name(const_cast<char *>("%rax"));
    names[ParseNode::RCX] = name_map.name(const_cast<char *>("%rcx"));
    names[ParseNode::RDX] = name_map.name(const_cast<char *>("%rdx"));
    names[ParseNode::RBX] = name_map.name(const_cast<char *>("%rbx"));
    names[ParseNode::RSP] = name_map.name(const_cast<char *>("%rsp"));
    names[ParseNode::RBP] = name_map.name(const_cast<char *>("%rbp"));
    names[ParseNode::RSI] = name_map.name(const_cast<char *>("%rsi"));
    names[ParseNode::RDI] = name_map.name(const_cast<char *>("%rdi"));
    names[ParseNode::R8] = name_map.name(const_cast<char *>("%r8"));
    names[ParseNode::R9] = name_map.name(const_cast<char *>("%r9"));
    names[ParseNode::R10] = name_map.name(const_cast<char *>("%r10"));
    names[ParseNode::R11] = name_map.name(const_cast<char *>("%r11"));
    names[ParseNode::R12] = name_map.name(const_cast<char *>("%r12"));
    names[ParseNode::R13] = name_map.name(const_cast<char *>("%r13"));
    names[ParseNode::R14] = name_map.name(const_cast<char *>("%r14"));
    names[ParseNode::HALT] = name_map.name(const_cast<char *>("halt"));
    names[ParseNode::NOP] = name_map.name(const_cast<char *>("nop"));
    names[ParseNode::RRMOVQ] = name_map.name(const_cast<char *>("rrmovq"));
    names[ParseNode::CMOVLE] = name_map.name(const_cast<char *>("cmovle"));
    names[ParseNode::CMOVL] = name_map.name(const_cast<char *>("cmovl"));
    names[ParseNode::CMOVE] = name_map.name(const_cast<char *>("cmove"));
    names[ParseNode::CMOVNE] = name_map.name(const_cast<char *>("cmovne"));
    names[ParseNode::CMOVGE] = name_map.name(const_cast<char *>("cmovge"));
    names[ParseNode::CMOVG] = name_map.name(const_cast<char *>("cmovg"));
    names[ParseNode::IRMOVQ] = name_map.name(const_cast<char *>("irmovq"));
    names[ParseNode::RMMOVQ] = name_map.name(const_cast<char *>("rmmovq"));
    names[ParseNode::MRMOVQ] = name_map.name(const_cast<char *>("mrmovq"));
    names[ParseNode::ADDQ] = name_map.name(const_cast<char *>("addq"));
    names[ParseNode::SUBQ] = name_map.name(const_cast<char *>("subq"));
    names[ParseNode::ANDQ] = name_map.name(const_cast<char *>("andq"));
    names[ParseNode::XORQ] = name_map.name(const_cast<char *>("xorq"));
    names[ParseNode::JMP] = name_map.name(const_cast<char *>("jmp"));
    names[ParseNode::JLE] = name_map.name(const_cast<char *>("jle"));
    names[ParseNode::JL] = name_map.name(const_cast<char *>("jl"));
    names[ParseNode::JE] = name_map.name(const_cast<char *>("je"));
    names[ParseNode::JNE] = name_map.name(const_cast<char *>("jne"));
    names[ParseNode::JGE] = name_map.name(const_cast<char *>("jge"));
    names[ParseNode::JG] = name_map.name(const_cast<char *>("jg"));
    names[ParseNode::CALL] = name_map.name(const_cast<char *>("call"));
    names[ParseNode::RET] = name_map.name(const_cast<char *>("ret"));
    names[ParseNode::PUSHQ] = name_map.name(const_cast<char *>("pushq"));
    names[ParseNode::POPQ] = name_map.name(const_cast<char *>("popq"));
    names[ParseNode::POS] = name_map.name(const_cast<char *>("pos"));
    names[ParseNode::ALIGN] = name_map.name(const_cast<char *>("align"));
    names[ParseNode::QUAD] = name_map.name(const_cast<char *>("quad"));

}

std::vector<Parser::ParseNode>
Parser::parse(const std::vector<Token>& tokvec)
{
    std::vector<ParseNode> ret;
    int i = 0;
    while (i < tokvec.size()) {
        auto tok = tokvec[i];
        switch (tok.t) {

            case Token::Type::INTEGER:
                // We should never just run into an Integer
                parse_error("Unexpected Integer found (", tok.u.value, "). ", tok.loc, "\n");

            case Token::Type::IDENTIFIER: {
                // Possible ParseNode Types that this has to parse:
                //      - Label
                //      - Instruction.
                auto node = parse_identifier(tokvec, i);
                i += node.len;
                ret.emplace_back(node);
                break;
            }

            case Token::Type::IMMEDIATE:
                auto node = parse_immediate(tokvec, i);
                i += node.len;
                ret.emplace_back(node);
                break;
        }
    }
    return ret;
}


Parser::ParseNode
Parser::parse_immediate(const std::vector<Token>& tokvec, int index)
{

    auto tok = tokvec[index];
    switch (tok.u.im) {

        case Token::Immediate::PERIOD: {
            // We now parse an assembler directive, either of .pos, .align, .quad
            if (index + 2 >= tokvec.size()) {
                parse_error("Incomplete directive found. ", tok.loc, "\n");
            }
            ParseNode node { .index = index, .len = 3 };
            // Check for the three possible directives
//            log(&tokvec[index+1].u.name, "\n", &names[ParseNode::POS], "\n");
            if (tokvec[index + 1].u.name == names[ParseNode::POS]) {
                node.t = ParseNode::Type::POS;
            } else if (tokvec[index + 1].u.name == names[ParseNode::ALIGN]) {
                node.t = ParseNode::Type::ALIGN;
            } else if (tokvec[index + 1].u.name == names[ParseNode::QUAD]) {
                node.t = ParseNode::Type::QUAD;
            } else {
                parse_error("Invalid directive found ", tokvec[index+1].u.name, ". ", tokvec[index+1].loc, "\n");
            }

            // Parse integer value
            if (tokvec[index + 2].t != Token::Type::INTEGER) {
                parse_error("Invalid directive found. Expected integral value. ", tokvec[index+2].loc, "\n");
            }
            return node;
        }

        case Token::Immediate::COMMA:
            // This can just be tossed away, but we should never just run into it beacuse it will be consumed by the
            // parser for an identifier which parses an instruction (which can contain a comma)
        case Token::Immediate::COLON:
            // Should never just find this because it should only be found by the label parser
        case Token::Immediate::DOLLAR_SIGN:
            // Should never be found here, should only ever be found in the instruction parser.
            parse_error("Invalid immediate found. ", tokvec[index].loc, "\n");
    }
}

Parser::ParseNode
Parser::parse_identifier(const std::vector<Token> &tokvec, int index)
{
    auto tok = tokvec[index];

    // Check if tokvec[index] matches one of the instructions.
    auto name_index = std::find(names.begin(), names.end(), tok.u.name) - names.begin();
    if (name_index >=  ParseNode::HALT && name_index <= ParseNode::POPQ)
    {
        return parse_instruction(tokvec, index, static_cast<ParseNode::NameIndices>(name_index));
    }

    // We are parsing a Label
    ParseNode node = { .index = index, .len = 2, .t = ParseNode::Type::LABEL };
    if (index+1 >= tokvec.size() || tokvec[index+1].u.im != Token::Immediate::COLON) {
        parse_error("Invalid identifier found: ", tokvec[index].u.name, ". ", tokvec[index].loc, "\n");
    }
    return node;
}


void
Parser::parse_registers(const std::vector<Token>& tokvec, int index)
{
    if (index + 2 >= tokvec.size()) {
        parse_error("Failed instruction parsing. Instruction too short. ", tokvec[index].loc, "\n");
    }

    if (tokvec[index].t != Token::Type::IDENTIFIER) {
        parse_error("Failed instruction parsing. Invalid register name found. ", tokvec[index].loc, "\n");
    }
    if (tokvec[index+1].t != Token::Type::IMMEDIATE && tokvec[index].u.im != Token::Immediate::COMMA) {
        parse_error("Failed instruction parsing. Comma not found. ", tokvec[index+1].loc, "\n");
    }
    if (tokvec[index+2].t != Token::Type::IDENTIFIER) {
        parse_error("Failed instruction parsing. Invalid register name found. ", tokvec[index+2].loc, "\n");
    }
}


Parser::ParseNode
Parser::parse_instruction(const std::vector<Token> &tokvec,
                          int i,
                          Parser::ParseNode::NameIndices name_index)
{
    Parser::ParseNode node = { .index = i, .t = ParseNode::Type::INSTRUCTION };
    switch (name_index) {

        case Parser::ParseNode::HALT:
            node.u.ins = Parser::ParseNode::Instruction::HALT;
            node.len = 1;
            return node;

        case Parser::ParseNode::NOP:
            node.u.ins = Parser::ParseNode::Instruction::NOP;
            node.len = 1;
            return node;

        case Parser::ParseNode::RRMOVQ:
            // Indentifier "," Identifier
            parse_registers(tokvec, i + 1);
            node.u.ins = Parser::ParseNode::Instruction::RRMOVQ;
            node.len = 4;
            return node;

        case Parser::ParseNode::CMOVLE:
            // Identifier "," Identifier
            parse_registers(tokvec, i + 1);
            node.u.ins = Parser::ParseNode::Instruction::CMOVLE;
            node.len = 4;
            return node;

        case Parser::ParseNode::CMOVL:
            // Identifier "," Identifier
            parse_registers(tokvec, i + 1);
            node.u.ins = Parser::ParseNode::Instruction::CMOVL;
            node.len = 4;
            return node;

        case Parser::ParseNode::CMOVE:
            // Identifier "," Identifier
            parse_registers(tokvec, i + 1);
            node.u.ins = Parser::ParseNode::Instruction::CMOVE;
            node.len = 4;
            return node;

        case Parser::ParseNode::CMOVNE:
            // Identifier "," Identifier
            parse_registers(tokvec, i + 1);
            node.u.ins = Parser::ParseNode::Instruction::CMOVNE;
            node.len = 4;
            return node;

        case Parser::ParseNode::CMOVGE:
            // Identifier "," Identifier
            parse_registers(tokvec, i + 1);
            node.u.ins = Parser::ParseNode::Instruction::CMOVGE;
            node.len = 4;
            return node;

        case Parser::ParseNode::CMOVG:
            // Identifier "," Identifier
            parse_registers(tokvec, i + 1);
            node.u.ins = Parser::ParseNode::Instruction::CMOVG;
            node.len = 4;
            return node;

        case Parser::ParseNode::IRMOVQ:
            // "$", (Integer or Identifier), ",", Identifier
            error("TODO: Function: ", __FUNCTION__, " Line Number: ", __LINE__, "\n");
            break;
        case Parser::ParseNode::RMMOVQ:
            // Identifier, ",", Integer, "(", Ident, ")"
            error("TODO: Function: ", __FUNCTION__, " Line Number: ", __LINE__, "\n");
            break;
        case Parser::ParseNode::MRMOVQ:
            // Integer, "(", Ident, ")", "," Ident
            error("TODO: Function: ", __FUNCTION__, " Line Number: ", __LINE__, "\n");
            break;
        case Parser::ParseNode::ADDQ:
            // Identifier "," Identifier
            error("TODO: Function: ", __FUNCTION__, " Line Number: ", __LINE__, "\n");
            break;
        case Parser::ParseNode::SUBQ:
            // Identifier "," Identifier
            error("TODO: Function: ", __FUNCTION__, " Line Number: ", __LINE__, "\n");
            break;
        case Parser::ParseNode::ANDQ:
            // Identifier "," Identifier
            error("TODO: Function: ", __FUNCTION__, " Line Number: ", __LINE__, "\n");
            break;
        case Parser::ParseNode::XORQ:
            // Identifier "," Identifier
            error("TODO: Function: ", __FUNCTION__, " Line Number: ", __LINE__, "\n");

            break;
        case Parser::ParseNode::JMP:
            error("TODO: Function: ", __FUNCTION__, " Line Number: ", __LINE__, "\n");
            break;
        case Parser::ParseNode::JLE:
            error("TODO: Function: ", __FUNCTION__, " Line Number: ", __LINE__, "\n");

            break;
        case Parser::ParseNode::JL:
            error("TODO: Function: ", __FUNCTION__, " Line Number: ", __LINE__, "\n");

            break;
        case Parser::ParseNode::JE:
            error("TODO: Function: ", __FUNCTION__, " Line Number: ", __LINE__, "\n");

            break;
        case Parser::ParseNode::JNE:
            error("TODO: Function: ", __FUNCTION__, " Line Number: ", __LINE__, "\n");

            break;
        case Parser::ParseNode::JGE:
            error("TODO: Function: ", __FUNCTION__, " Line Number: ", __LINE__, "\n");

            break;
        case Parser::ParseNode::JG:
            error("TODO: Function: ", __FUNCTION__, " Line Number: ", __LINE__, "\n");

            break;
        case Parser::ParseNode::CALL:
            error("TODO: Function: ", __FUNCTION__, " Line Number: ", __LINE__, "\n");

            break;
        case Parser::ParseNode::RET:
            error("TODO: Function: ", __FUNCTION__, " Line Number: ", __LINE__, "\n");

            break;
        case Parser::ParseNode::PUSHQ:
            error("TODO: Function: ", __FUNCTION__, " Line Number: ", __LINE__, "\n");

            break;
        case Parser::ParseNode::POPQ:
            error("TODO: Function: ", __FUNCTION__, " Line Number: ", __LINE__, "\n");

            break;

        default:
            parse_error("Invalid Instruction found. ", tokvec[i].loc, "\n");
    }


    return {};
}
