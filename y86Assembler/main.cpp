#include <iostream>

#include "lexer.h"
#include "parser.h"
#include "error_handling.h"

int main(int argc, char** argv)
{
    if (argc != 2) {
        error("expected usage: y86Assembler <assembly_file>\n");
    }
    std::filesystem::path p(argv[1]);

    auto lexer = Lexer(); // NOTE: We need to keep lexer around for the name map
    auto res = lexer.lex(p);
    auto parser = Parser(std::move(lexer.name_map));
    auto pnode_vec = parser.parse(res);

}