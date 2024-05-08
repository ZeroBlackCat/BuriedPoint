#include "gtest/gtest.h"
#include "include/buried.h"

TEST(BuriedBasicTest, Test1) { EXPECT_EQ(Buried_Test(), 1); }

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}