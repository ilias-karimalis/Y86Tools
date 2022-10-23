#include "token.h"

std::ostream&
operator<<(std::ostream& stream, const Token& token)
{
    stream << token.t << ", Source File: " << token.loc.path << ", Row: " << token.loc.row;
    stream << ", Col: " << token.loc.col << ", Value: ";
    switch (token.t) {

        case Token::Type::INTEGER:
            stream << token.u.value;
            break;
        case Token::Type::IDENTIFIER:
        case Token::Type::IMMEDIATE:
            stream << token.u.name;
            break;
    }
    return stream;
}

std::ostream&
operator<<(std::ostream& stream, const Token::Type& t)
{
    switch (t) {

        case Token::Type::INTEGER:
            stream << "Token::Type::INTEGER";
            return stream;

//        case Token::Type::KEYWORD:
//            stream << "Token::Type::KEYWORD";
//            return stream;

        case Token::Type::IDENTIFIER:
            stream << "Token::Type::IDENTIFIER";
            return stream;

        case Token::Type::IMMEDIATE:
            stream << "Token::Type::IMMEDIATE";
            return stream;
    }
}

std::ostream &operator<<(std::ostream& stream, const Token::Location& loc) {
    stream << "In Source File: " << loc.path << " Row, Col: " << loc.row << ", " << loc.col << ".\n";
    return stream;
}
