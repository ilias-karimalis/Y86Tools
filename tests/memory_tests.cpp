#include "gtest/gtest.h"
#include "processor.h"

TEST(MemoryTests, MemoryOutOfBoundsReadWriteByte)
{
    // given:
    size_t psize = 32;
    uint8_t arr[psize];
    Processor p = { arr, psize };

    // when:
    auto read_val = p.memReadByte(psize);
    auto read_stat = p.status;
    p.status = Processor::Status::AOK;

    auto write_res = p.memWriteByte(psize, 'a');

    // then:
    EXPECT_FALSE(read_val.has_value());
    EXPECT_EQ(read_stat, Processor::Status::BadAddress);

    EXPECT_FALSE(write_res);
    EXPECT_EQ(p.status, Processor::Status::BadAddress);
}

TEST(MemoryTests, MemoryOutOfBoundsReadWriteWord)
{
    // given
    size_t psize = 32;
    uint8_t arr[psize];
    Processor p = { arr, psize };

    // when:
    for (auto addr = psize - 7; addr <= psize; addr++, p.status = Processor::Status::AOK) {

        auto read_val = p.memReadWord(addr);
        auto read_stat = p.status;
        p.status = Processor::Status::AOK;

        auto write_res = p.memWriteWord(addr, 139314124);

        // then:
        EXPECT_FALSE(read_val.has_value());
        EXPECT_EQ(read_stat, Processor::Status::BadAddress);

        EXPECT_FALSE(write_res);
        EXPECT_EQ(p.status, Processor::Status::BadAddress);

    }
}

TEST(MemoryTests, ReadWord)
{
    // given:
    size_t psize = 8;
    const char *arr = "DEADBEEFPEPEPOPO";
    Processor p = {(uint8_t *) arr, psize };

    // when:
    auto word = p.memReadWord(0);

    // then
    EXPECT_TRUE(word.has_value());
    EXPECT_EQ(word.value(), *(uint64_t*) arr );

}