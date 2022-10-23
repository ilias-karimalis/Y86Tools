#ifndef Y86EMULATOR_LOG_H
#define Y86EMULATOR_LOG_H

#include <iostream>

template<class... Args>
void log(Args... args)
{
    (std::cout << ... << args);
}


#endif //Y86EMULATOR_LOG_H
