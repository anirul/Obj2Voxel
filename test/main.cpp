#include <gtest/gtest.h>
#include "test/VoxelTest.h"
#include "test/ObjTest.h"

int main(int ac, char** av)
{
	::testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}
