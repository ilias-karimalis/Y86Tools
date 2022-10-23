#include <iostream>
#include <fstream>

#include "lexer.h"
#include "error_handling.h"
#include "logging.h"

std::vector<Token> Lexer::lex(const std::filesystem::path& p) {

    // Open File as a list
    // Walk the file tokenizing the input, effectively it's just a big switch
    // case over char values.w
    std::ifstream source_stream(p);
    if (!source_stream) {
        lex_error("File: ", p, " did not open.\n");
    }

    std::string source_string((std::istreambuf_iterator<char>(source_stream)), std::istreambuf_iterator<char>());
    char* source_file = source_string.data();
    std::vector<Token> tokvec;
    auto file_len = std::strlen(source_file);

    log("File Length in bytes: ", file_len, "\n");

    int row = 1;
    int col = 1;
    for (auto i = 0; i < file_len;) {
        auto c = source_file[i];

        switch (c) {

            case '\t':
            case ' ':
                i++;
                col++;
                break;

            case '\n':
                i++;
                col = 0;
                row += 1;
                break;

            case '#':
                // Parsing a comment:
                while (i < file_len && source_file[i] != '\n') {
                    i += 1;
                    col += 1;
                }
                if (i == file_len) {
                    break;
                }
                // We've arrived at a new line.
                i += 1;
                col = 0;
                row += 1;
                break;

            case '.': {
                Token tok;
                tok.t = Token::Type::IMMEDIATE;
                tok.loc = {p, row, col};
                tok.u.im = Token::Immediate::PERIOD;
                tokvec.emplace_back(std::move(tok));
                i += 1;
                col += 1;
            }
                break;

            case ':': {
                Token tok;
                tok.t = Token::Type::IMMEDIATE;
                tok.loc = {p, row, col};
                tok.u.im = Token::Immediate::COLON;
                tokvec.emplace_back(std::move(tok));
                i += 1;
                col += 1;
                break;
            }

            case ',': {
                Token tok;
                tok.t = Token::Type::IMMEDIATE;
                tok.loc = {p, row, col};
                tok.u.im = Token::Immediate::COMMA;
                tokvec.emplace_back(std::move(tok));
                i += 1;
                col += 1;
                break;
            }

            case '$': {
                Token tok;
                tok.t = Token::Type::IMMEDIATE;
                tok.loc = {p, row, col};
                tok.u.im = Token::Immediate::DOLLAR_SIGN;
                tokvec.emplace_back(std::move(tok));
                i += 1;
                col += 1;
                break;
            }

            case '(':
                todo("Implement '(' lexing");
                break;

            case ')':
                todo("Implement ')' lexing");
                break;

            case '0':
                if (i + 2 < file_len && (source_file[i + 1] == 'x' || source_file[i + 1] == 'X')) {
                    // Parse Hex Number...
                    uint64_t value = 0;
                    auto n_digits = 0;
                    auto start_col = col;
                    Token::Location loc = { p, row, start_col };
                    col += 2;
                    i += 2;
                    while (ishexnumber(source_file[i])) {
                        n_digits += 1;
                        // handling the overflow test case:
                        if (n_digits > 16) {
                            lex_error("Integer being lexed is larger than 64 byte maximum. ", loc, "\n");
                        }
                        c = source_file[i];
                        if (c >= '0' && c <= '9') {
                            value = (value << 4) | ((c - '0') & 0xF);
                        } else if (c >= 'a' && c <= 'f') {
                            value = (value << 4) | ((c - 'a' + 10) & 0xF);
                        } else if (c >= 'A' && c <= 'F') {
                            value = (value << 4) | ((c - 'A' + 10) & 0x0F);
                        } else {
                            lex_error("Lexing hexadecimal integer contains non hex number. ", loc, "\n");
                        }
                        i++;
                        col++;
                    }

                    // when parsing integers, the next character has to be one of, '#', ' ', '\n' or the end of the file.
                    if (i >= file_len || source_file[i] == '#' || source_file[i] == '\n' || source_file[i] == ' ') {
                        // We can now construct a Token:
                        Token tok;
                        tok.t = Token::Type::INTEGER;
                        tok.loc = loc;
                        tok.u.value = value;
                        tokvec.emplace_back(std::move(tok));
                    } else {
                        lex_error("Failed lexing hex number, unexpected character found. ", loc, "\n");
                    }
                    break;
                }
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': {
                // Parse decimal (base 10) number
                // What we really want to do is walk through the digits and build our integer, view this as a custom atoi
                // We need error checking to see that we don't have an integer too large to store in a uint64_t

                uint64_t value = 0;
                Token::Location loc = { p, row, col };
                while (isdigit(source_file[i])) {
                    // handling the overflow test case:
                    if (value > UINT64_MAX / 10
                        || (value == UINT64_MAX / 10 && source_file[i] - '0' > 7)) {
                        // We have overflowed!!! this integer is not allowed.
                        lex_error("Integer being lexed is larger than 64 byte maximum. ", loc, "\n");
                    }
                    value = 10 * value + (source_file[i] - '0');
                    i++;
                    col++;
                }
                Token tok;
                tok.t = Token::Type::INTEGER;
                tok.loc = loc;
                tok.u.value = value;
                tokvec.emplace_back(std::move(tok));
            }
                break;

            case '%':
            case 'a'...'z':
            case 'A'...'Z': // Gcc Language extension non-portable c++ will need to change later
            {
                int start_col = col;
                int start_index = i;
                // Now we will need the string hash map! and unique naming.
                if (source_file[i] == '%') {
                    i++;
                    col++;
                }
                while (isalnum(source_file[i])) {
                    i++;
                    col++;
                }
                Token tok;
                tok.t = Token::Type::IDENTIFIER;
                tok.loc = {p, row, start_col};
                // This is a hack, put '\0' in source_file[i] where i is right after our string, so that we can use
                // our set to check the name.
                // TODO I think this works, but I should test it better!
                char temp = source_file[i];
                source_file[i] = '\0';
                tok.u.name = name_map.name(&(source_file[start_index]));
                source_file[i] = temp;
                tokvec.emplace_back(std::move(tok));
            }
                break;

            default: {
                Token::Location loc = { p, row, col };
                lex_error("Illegal Token Parsed (", c, "). ", loc, "\n");
            }

        }

    }

    return tokvec;
}

