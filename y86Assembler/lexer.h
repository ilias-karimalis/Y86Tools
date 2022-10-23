#ifndef Y86EMULATOR_LEXER_HPP
#define Y86EMULATOR_LEXER_HPP

#include <filesystem>
#include <vector>
#include <set>
#include <cstring>

#include "token.h"
#include "namemap.h"

struct Lexer {

    std::vector<Token> lex(const std::filesystem::path& p);

    NameMap name_map;
};


#endif //Y86EMULATOR_LEXER_HPP
