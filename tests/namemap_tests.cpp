#include "gtest/gtest.h"
#include "namemap.h"

TEST(asd, asd)
{
    // given:
    NameMap nm;
    char s1[] = "asd";
    char s2[] = "asd";
    char s3[] = "asdfg";

    // when:
    const char * p1 = nm.name(s1);
    const char * p2 = nm.name(s2);
    const char * p3 = nm.name(s3);

    // then:
    EXPECT_EQ(p1, p2);
    EXPECT_NE(p2, p3);
}


