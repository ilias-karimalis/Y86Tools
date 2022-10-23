#ifndef Y86EMULATOR_PROCESSOR_H
#define Y86EMULATOR_PROCESSOR_H

#include <optional>

#include "instruction.h"

struct Processor {

    enum class Status {
        AOK = 0x1,
        Halt = 0x2,
        BadAddress = 0x3,
        InvalidInstruction = 0x4
    };

    Processor(uint8_t* program, uint64_t program_size);

    std::optional<uint8_t> memReadByte(uint64_t address);
    std::optional<uint64_t> memReadWord(uint64_t address);

    bool memWriteByte(uint64_t address, uint8_t value);
    bool memWriteWord(uint64_t address, uint64_t value);

    std::optional<Instruction> fetchInstruction();
    void executeInstruction(Instruction instruction); // Maybe rename stepInstruction
    bool evaluateCondition(Condition condition) const;
    uint64_t evaluateOperation(Register reg_a, Register reg_b, Operation op);

    uint8_t* program_map;
    uint64_t program_size;
    uint64_t program_counter;
    Status status;
    uint64_t register_file[16] = {};

    bool zero_flag;
    bool signed_flag;
    bool overflow_flag;

    void reset();
};

#endif //Y86EMULATOR_PROCESSOR_H
