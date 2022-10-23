#ifndef Y86EMULATOR_LOG_H
#define Y86EMULATOR_LOG_H

#include <iostream>

#include "error_handling.h"

template<class... Args>
void log(Args... args)
{
    (std::cout << ... << args);
}

template<class... Args>
void logn(Args... args)
{
    (std::cout << ... << args) << '\n';
}

#define todo(...) error("[TODO (function: ", __FUNCTION__, "), (line number: ", __LINE__, ")]: ", __VA_ARGS__)

#endif //Y86EMULATOR_LOG_H
