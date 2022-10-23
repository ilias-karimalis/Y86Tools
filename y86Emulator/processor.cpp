#include <iostream>
#include "processor.h"


Processor::Processor(uint8_t *program, uint64_t program_size)
    : program_map(program), program_size(program_size), status(Status::AOK),
      program_counter(0), zero_flag(false), signed_flag(false), overflow_flag(false)
{}

std::optional<uint8_t>
Processor::memReadByte(uint64_t address)
{
    if (address >= program_size) {
        status = Status::BadAddress;
        return {};
    }
    return program_map[address];
}

std::optional<uint64_t>
Processor::memReadWord(uint64_t address)
{
    if (address + 7 >= program_size) {
        status = Status::BadAddress;
        return {};
    }
    uint64_t ret = 0;
    for (auto i = 7; i >= 0; i--) {
        ret <<= 8;
        ret += program_map[address + i];
    }
    return ret;
}

bool
Processor::memWriteByte(uint64_t address, uint8_t value)
{
    if (address >= program_size) {
        status = Status::BadAddress;
        return false;
    }
    program_map[address] = value;
    return true;
}

bool
Processor::memWriteWord(uint64_t address, uint64_t value)
{
    if (address + 7 >= program_size) {
        status = Status::BadAddress;
        return false;
    }
    for (auto i = 0; i < 8; i++) {
        program_map[address + i] = (value >> (8 * i)) & 0x0FF;
    }
    return true;
}

std::optional<Instruction> Processor::fetchInstruction() {

    Instruction ret;
    ret.location = program_counter;

    auto first_byte =  memReadByte(program_counter);
    if (!first_byte.has_value()) {
        return {};
    }
    ret.opcode = static_cast<OpCode>((first_byte.value() >> 4) & 0x0F);
    // We will interpret this as either an Operation or a Condition based on the
    // instruction type opCode when it's used in execution, but for now this is
    // fine for storage;
    ret.func_code = static_cast<Condition>(first_byte.value() & 0x0F);

    std::optional<uint8_t> registers;
    switch(ret.opcode) {

        // 1 Byte Instructions
        case OpCode::HALT:
        case OpCode::NOP:
        case OpCode::RET:
//            if (ret.func_code != Condition::NoComparison) {
//                status = Status::InvalidInstruction;
//                return {};
//            }
            return ret;

        // 2 Byte Instructions
        case OpCode::RRMVXX:
        case OpCode::OPQ:
            if (ret.func_code > Condition::NotEqual) {
                status = Status::InvalidInstruction;
                return {};
            }
            registers = memReadByte(program_counter + 1);
            if (!registers.has_value()) {
                status = Status::InvalidInstruction;
                return {};
            }
            ret.reg_a = static_cast<Register>((registers.value() >> 4) & 0x0F);
            ret.reg_b = static_cast<Register>(registers.value() & 0x0F);
            return ret;

        case OpCode::PUSH:
        case OpCode::POP:
//            if (ret.func_code != Condition::NoComparison) {
//                status = Status::InvalidInstruction;
//                return {};
//            }
            registers = memReadByte(program_counter + 1);
            if (!registers.has_value()) {
                status = Status::InvalidInstruction;
                return {};
            }
            ret.reg_a = static_cast<Register>((registers.value() >> 4) & 0x0F);
            ret.reg_b = static_cast<Register>(registers.value() & 0x0F);
            return ret;

        // 9 Byte Instructions
        case OpCode::JUMP:
//            if (ret.func_code != Condition::NoComparison) {
//                status = Status::InvalidInstruction;
//                return {};
//            }
        case OpCode::CALL:
            ret.val_c = memReadWord(program_counter + 1);
            if (!ret.val_c.has_value()) {
                status = Status::InvalidInstruction;
                return {};
            }
            return ret;

        // 10 Byte Instructions
        case OpCode::IRMOVQ:
        case OpCode::RMMOVQ:
        case OpCode::MRMOVQ:
//            if (ret.func_code != Condition::NoComparison) {
//                status = Status::InvalidInstruction;
//                return {};
//            }
            registers = memReadByte(program_counter + 1);
            ret.val_c = memReadWord(program_counter + 2);
            if (!registers.has_value() || !ret.val_c.has_value()) {
                status = Status::InvalidInstruction;
                return {};
            }
            ret.reg_a = static_cast<Register>((registers.value() >> 4) & 0x0F);
            ret.reg_b = static_cast<Register>(registers.value() & 0x0F);
            return ret;

        // Incorrect Opcode
        default:
            status = Status::InvalidInstruction;
            return {};
    }
}

void Processor::executeInstruction(Instruction instruction) {
    switch (instruction.opcode) {

        case OpCode::HALT: {
            status = Status::Halt;
            return;
        }

        case OpCode::NOP: {
            program_counter += instructionLength(OpCode::NOP);
            return;
        }

        case OpCode::RRMVXX: {
            bool cond = evaluateCondition(instruction.func_code);
            if (cond) {
                register_file[static_cast<size_t>(*instruction.reg_b)] = register_file[static_cast<size_t>(*instruction.reg_a)];
            }
            program_counter += instructionLength(OpCode::RRMVXX);
            return;
        }

        case OpCode::IRMOVQ: {
            register_file[static_cast<size_t>(*instruction.reg_b)] = *instruction.val_c;
            program_counter += instructionLength(OpCode::IRMOVQ);
            return;
        }

        case OpCode::RMMOVQ: {
            auto val_a = register_file[static_cast<size_t>(*instruction.reg_a)];
            auto val_b = register_file[static_cast<size_t>(*instruction.reg_b)];
            auto val_e = val_b + instruction.val_c.value();
            memWriteWord(val_e, val_a);
            program_counter += instructionLength(OpCode::RMMOVQ);
            return;
        }

        case OpCode::MRMOVQ: {
            auto val_b = register_file[static_cast<size_t>(*instruction.reg_b)];
            auto val_e = val_b + instruction.val_c.value();
            auto read = memReadWord(val_e);
            if (!read.has_value()) {
                // Not sure what we should do in the case where val_e is not within
                // the bounds of memory?
                return;
            }
            register_file[static_cast<size_t>(*instruction.reg_a)] = read.value();
            program_counter += instructionLength(OpCode::MRMOVQ);
            return;
        }

        case OpCode::OPQ: {
            auto op = static_cast<Operation>(instruction.func_code);
            auto val_e = evaluateOperation(*instruction.reg_a, *instruction.reg_b, op);
            register_file[static_cast<size_t>(*instruction.reg_b)] = val_e;
            program_counter += instructionLength(OpCode::OPQ);
        }

        case OpCode::JUMP: {
            bool cond = evaluateCondition(instruction.func_code);
            if (cond) {
                program_counter = instruction.val_c.value();
            } else {
                program_counter += instructionLength(OpCode::JUMP);
            }
            return;
        }

        case OpCode::CALL: {
            auto val_b = register_file[static_cast<size_t>(Register::RSP)];
            auto val_e = val_b - 8;
            if (!memWriteWord(val_e, program_counter + instructionLength(OpCode::CALL))){
                return;
            }
            register_file[static_cast<size_t>(Register::RSP)] = val_e;
            program_counter = instruction.val_c.value();
            return;
        }

        case OpCode::RET: {
            auto val_a = register_file[static_cast<size_t>(Register::RSP)];
            auto val_m = memReadWord(val_a);
            register_file[static_cast<size_t>(Register::RSP)] += 8;
            if (!val_m.has_value()) {
                // Some kind of error handling!!
                return;
            }
            program_counter = val_m.value();
            return;
        }

        case OpCode::PUSH: {
            auto val_a = register_file[static_cast<size_t>(*instruction.reg_a)];
            auto val_b = register_file[static_cast<size_t>(Register::RSP)];
            if (!memWriteWord(val_b - 8, val_a)) {
                return;
            }
            register_file[static_cast<size_t>(Register::RSP)] -= 8;
            program_counter += instructionLength(OpCode::PUSH);
            return;
        }

        case OpCode::POP: {
            auto val_b = register_file[static_cast<size_t>(Register::RSP)];
            auto val_m = memReadWord(val_b);
            if (!val_m.has_value()) {
                return;
            }
            register_file[static_cast<size_t>(*instruction.reg_a)] = val_m.value();
            register_file[static_cast<size_t>(Register::RSP)] += 8;
            program_counter += instructionLength(OpCode::POP);
            return;
        }

        default:
            status = Status::InvalidInstruction;
            return;
    }

}

bool Processor::evaluateCondition(Condition condition) const {
    switch (condition) {

        case Condition::NoComparison:
            return true;
        case Condition::LessEqual:
            return zero_flag || signed_flag;

        case Condition::Less:
            return signed_flag && !zero_flag;

        case Condition::Equal:
            return zero_flag;

        case Condition::NotEqual:
            return !zero_flag;

        case Condition::GreaterEqual:
            return zero_flag || !signed_flag;

        case Condition::Greater:
            return !signed_flag && !zero_flag;
    }
}

uint64_t Processor::evaluateOperation(Register reg_a, Register reg_b, Operation op) {
    // TODO
    return 0;
}

void Processor::reset() {
    // Reset the processor state
    program_counter = 0;
    status = Status::AOK;
    for (auto& reg : register_file) {
        reg = 0;
    }
    zero_flag = signed_flag = overflow_flag = false;
}



