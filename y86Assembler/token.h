#ifndef Y86EMULATOR_TOKEN_HPP
#define Y86EMULATOR_TOKEN_HPP

#include <filesystem>

struct Token {

    struct Location {
        std::filesystem::path path;
        int row;
        int col;
    };

    enum class Type {
        INTEGER,
        IDENTIFIER,
        IMMEDIATE,
    };

    enum class Immediate {
        PERIOD,
        COMMA,
        COLON,
        DOLLAR_SIGN
    };

    enum class InstructionName {

    };

    friend std::ostream& operator<<(std::ostream& stream, const Location& loc);
    friend std::ostream& operator<<(std::ostream& stream, const Type& t);

    Type t;
    Location loc;
    union {
        const char* name;
        uint64_t value;
        Immediate im;
    } u;

    friend std::ostream& operator<<(std::ostream& stream, const Token& token);

};


#endif //Y86EMULATOR_TOKEN_HPP
