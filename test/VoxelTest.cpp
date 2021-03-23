#include "VoxelTest.h"
#include <algorithm>

namespace test {

	TEST_F(VoxelTest, CreateTest)
	{
		ASSERT_TRUE(obj_ptr_);
		voxel_ptr_ = std::make_unique<voxel::Voxel>(
			*obj_ptr_, 
			glm::vec3{ 8.f, 8.f, 8.f });
		EXPECT_TRUE(voxel_ptr_);
	}

	TEST_F(VoxelTest, InsiderTest)
	{
		ASSERT_TRUE(obj_ptr_);
		voxel_ptr_ = std::make_unique<voxel::Voxel>(
			*obj_ptr_,
			glm::vec3{ 8.f, 8.f, 8.f });
		ASSERT_TRUE(voxel_ptr_);
		voxel::proto::VoxelFile voxel_file = voxel_ptr_->GetVoxelFile();
		// SaveProtoToJsonFile(voxel_file, "./Apple.json");
		// Compute the max length of a diagonal.
		glm::vec3 end =	glm::vec3(
				voxel_file.end_x(),
				voxel_file.end_y(),
				voxel_file.end_z());
		glm::vec3 begin = glm::vec3(
				voxel_file.begin_x(),
				voxel_file.begin_y(),
				voxel_file.begin_z());
		float max_diag = glm::length(begin - end);
		std::for_each(
			voxel_file.data().cbegin(), 
			voxel_file.data().cend(), 
			[max_diag](float f) 
		{
			EXPECT_GT(max_diag, f);
		});
	}

} // End namespace test.
