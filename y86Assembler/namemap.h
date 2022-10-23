#ifndef Y86EMULATOR_NAMEMAP_H
#define Y86EMULATOR_NAMEMAP_H

#include <map>

struct NameMap {

    NameMap();

    const char * name(char * str);

    struct cstreq {
        bool operator()(const char* a, const char* b) const {
            return std::strcmp(a, b) < 0;
        }
    };

    std::map<char *, const char *, cstreq> nm;


};

#endif //Y86EMULATOR_NAMEMAP_H
