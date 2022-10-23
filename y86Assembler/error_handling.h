#ifndef Y86ASSEMBLER_ERROR_HANDLING_H
#define Y86ASSEMBLER_ERROR_HANDLING_H

#include <iostream>

template<class... Args>
[[noreturn]] void error(Args... args)
{
    (std::cout << ... << args);
    exit(-1);
}

template<class... Args>
[[noreturn]] void parse_error(Args... args)
{
    std::cout << "[Parse Error]: ";
    error(args...);
}

template<class... Args>
[[noreturn]] void lex_error(Args... args)
{
    std::cout << "[Lex Error]: ";
    error(args...);
}

template<class... Args>
[[noreturn]] void ast_error(Args... args)
{
    std::cout << "[Type Checking Error]: ";
    error(args...);
}


#endif // Y86ASSEMBLER_ERROR_HANDLING_H