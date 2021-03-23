#include "VoxelTest.h"

namespace test {

	TEST_F(VoxelTest, CreateTest)
	{
		ASSERT_TRUE(obj_ptr_);
		voxel_ptr_ = std::make_unique<voxel::Voxel>(
			*obj_ptr_, 
			glm::vec3{ 8.f, 8.f, 8.f });
		EXPECT_TRUE(voxel_ptr_);
	}

} // End namespace test.
