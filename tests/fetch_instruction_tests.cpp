#include "gtest/gtest.h"
#include "instruction.h"
#include "processor.h"
#include "opcode.h"

TEST(InstructionTests, FetchHALT)
{
    // given:
    size_t psize = 1;
    uint8_t pmap = 0;
    Processor p = { &pmap, psize };

    // when:
    auto instruction = p.fetchInstruction();

    // then:
    EXPECT_TRUE(instruction.has_value());
    EXPECT_EQ(instruction->opcode, OpCode::HALT);
    EXPECT_EQ(instruction->func_code, Condition::NoComparison); // Not sure about this one!
    EXPECT_FALSE(instruction->reg_a.has_value());
    EXPECT_FALSE(instruction->reg_b.has_value());
    EXPECT_FALSE(instruction->val_c.has_value());
    EXPECT_EQ(instruction->location, 0);
    EXPECT_EQ(instructionLength(OpCode::HALT), 1);

    EXPECT_EQ(p.status, Processor::Status::AOK);
}

TEST(InstructionTests, FetchNOP)
{
    // given:
    size_t psize = 1;
    uint8_t pmap = 0x10;
    Processor p = { &pmap, psize };

    // when:
    auto instruction = p.fetchInstruction();

    // then:
    EXPECT_TRUE(instruction.has_value());
    EXPECT_EQ(instruction->opcode, OpCode::NOP);
    EXPECT_EQ(instruction->func_code, Condition::NoComparison);
    EXPECT_FALSE(instruction->reg_a.has_value());
    EXPECT_FALSE(instruction->reg_b.has_value());
    EXPECT_FALSE(instruction->val_c.has_value());
    EXPECT_EQ(instruction->location, 0);
    EXPECT_EQ(instructionLength(OpCode::NOP), 1);

    EXPECT_EQ(p.status, Processor::Status::AOK);
}

TEST(InstructionTests, FetchRRMVXX)
{
    // given:
    size_t psize = 2;
    uint8_t pmap[] = { 0x20, 0x59 };
    Processor p = { pmap, psize };

    // when:
    auto instruction = p.fetchInstruction();

    // then
    EXPECT_TRUE(instruction.has_value());
    EXPECT_EQ(instruction->opcode, OpCode::RRMVXX);
    EXPECT_EQ(instruction->func_code, Condition::NoComparison);
    EXPECT_TRUE(instruction->reg_a.has_value());
    EXPECT_EQ(*instruction->reg_a, Register::RBP);
    EXPECT_TRUE(instruction->reg_b.has_value());
    EXPECT_EQ(*instruction->reg_b, Register::R9);
    EXPECT_FALSE(instruction->val_c.has_value());
    EXPECT_EQ(instruction->location, 0);
    EXPECT_EQ(instructionLength(OpCode::RRMVXX), 2);

    EXPECT_EQ(p.status, Processor::Status::AOK);
}

TEST(InstructionTests, FetchIRMOVQ)
{
    // given:
    size_t psize = 10;
    uint8_t pmap[] = { 0x30, 0xF4, 0xEF, 0xBE, 0xAD, 0xDE, 0xEF, 0xBE, 0xAD, 0xDE };
    Processor p = { pmap, psize };

    // when:
    auto instruction = p.fetchInstruction();

    // then:
    EXPECT_TRUE(instruction.has_value());
    EXPECT_EQ(instruction->opcode, OpCode::IRMOVQ);
    EXPECT_EQ(instruction->func_code, Condition::NoComparison);
    EXPECT_TRUE(instruction->reg_a.has_value());
    EXPECT_EQ(*instruction->reg_a, Register::RNONE);
    EXPECT_TRUE(instruction->reg_b.has_value());
    EXPECT_EQ(*instruction->reg_b, Register::RSP);
    EXPECT_TRUE(instruction->val_c.has_value());
    EXPECT_EQ(instruction->val_c.value(), *(uint64_t*)(&pmap[2]));
    EXPECT_EQ(instruction->location, 0);
    EXPECT_EQ(instructionLength(OpCode::IRMOVQ), 10);

    EXPECT_EQ(p.status, Processor::Status::AOK);
}

TEST(InstructionTests, FetchRMMOVQ)
{
    // given:
    size_t psize = 10;
    uint8_t pmap[] = { 0x40, 0x26, 0xEF, 0xBE, 0xAD, 0xDE, 0xEF, 0xBE, 0xAD, 0xDE };
    Processor p = { pmap, psize };

    // when:
    auto instruction = p.fetchInstruction();

    // then:
    EXPECT_TRUE(instruction.has_value());
    EXPECT_EQ(instruction->opcode, OpCode::RMMOVQ);
    EXPECT_EQ(instruction->func_code, Condition::NoComparison);
    EXPECT_TRUE(instruction->reg_a.has_value());
    EXPECT_EQ(*instruction->reg_a, Register::RDX);
    EXPECT_TRUE(instruction->reg_b.has_value());
    EXPECT_EQ(*instruction->reg_b, Register::RSI);
    EXPECT_TRUE(instruction->val_c.has_value());
    EXPECT_EQ(*instruction->val_c, *(uint64_t*)(&pmap[2]));
    EXPECT_EQ(instruction->location, 0);
    EXPECT_EQ(instructionLength(OpCode::RMMOVQ), 10);

    EXPECT_EQ(p.status, Processor::Status::AOK);
}

// TODO parsing seems to work add more if needed.