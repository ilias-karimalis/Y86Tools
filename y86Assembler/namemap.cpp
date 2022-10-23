#include "namemap.h"

const char *NameMap::name(char *str) {
    // What we want is a hash map of StringLiterals to const char *;
    auto res = nm.find(str);
    if (res == nm.end()) {
        char * cstr = static_cast<char *>(malloc(strlen(str) + 1));
        strcpy(cstr, str);
        return nm.insert({str, static_cast<const char *>(cstr)}).first->second;
    }
    return res->second;
}

NameMap::NameMap() {

    // Adding primitives to the Name Map
    name(const_cast<char *>("halt"));
    name(const_cast<char *>("nop"));
    name(const_cast<char *>("rrmovq"));
    name(const_cast<char *>("cmovle"));
    name(const_cast<char *>("cmovl"));
    name(const_cast<char *>("cmove"));
    name(const_cast<char *>("cmovne"));
    name(const_cast<char *>("cmovge"));
    name(const_cast<char *>("cmovg"));
    name(const_cast<char *>("irmovq"));
    name(const_cast<char *>("rmmovq"));
    name(const_cast<char *>("mrmovq"));
    name(const_cast<char *>("addq"));
    name(const_cast<char *>("subq"));
    name(const_cast<char *>("andq"));
    name(const_cast<char *>("xorq"));
    name(const_cast<char *>("jmp"));
    name(const_cast<char *>("jle"));
    name(const_cast<char *>("jl"));
    name(const_cast<char *>("je"));
    name(const_cast<char *>("jne"));
    name(const_cast<char *>("jge"));
    name(const_cast<char *>("jg"));
    name(const_cast<char *>("call"));
    name(const_cast<char *>("ret"));
    name(const_cast<char *>("pushq"));
    name(const_cast<char *>("popq"));
    name(const_cast<char *>("pos"));
    name(const_cast<char *>("align"));
    name(const_cast<char *>("quad"));
    name(const_cast<char *>("%rax"));
    name(const_cast<char *>("%rcx"));
    name(const_cast<char *>("%rdx"));
    name(const_cast<char *>("%rbx"));
    name(const_cast<char *>("%rsp"));
    name(const_cast<char *>("%rbp"));
    name(const_cast<char *>("%rsi"));
    name(const_cast<char *>("%rdi"));
    name(const_cast<char *>("%r8"));
    name(const_cast<char *>("%r9"));
    name(const_cast<char *>("%r10"));
    name(const_cast<char *>("%r11"));
    name(const_cast<char *>("%r12"));
    name(const_cast<char *>("%r13"));
    name(const_cast<char *>("%r14"));
}

