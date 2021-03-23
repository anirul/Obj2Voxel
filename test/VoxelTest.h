#include <gtest/gtest.h>
#include "lib/FileSystem.h"
#include "lib/Voxel.h"

namespace test {

	class VoxelTest : public testing::Test
	{
	public:
		VoxelTest() 
		{
			// Load an apple as a test object.
			std::string path = voxel::FindPath("asset/models/");
			obj_ptr_ = std::make_unique<voxel::Obj>(path, "Apple.obj");
		}

	protected:
		std::unique_ptr<voxel::Obj> obj_ptr_ = nullptr;
		std::unique_ptr<voxel::Voxel> voxel_ptr_ = nullptr;
	};

} // End namespace test.
