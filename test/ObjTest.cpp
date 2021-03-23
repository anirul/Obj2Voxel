#include "ObjTest.h"

namespace test {

	TEST_F(ObjTest, CreationTest)
	{
		obj_ptr_ = std::make_unique<voxel::Obj>(path_, obj_file_);
		EXPECT_TRUE(obj_ptr_);
	}

	TEST_F(ObjTest, MeshTest)
	{
		obj_ptr_ = std::make_unique<voxel::Obj>(path_, obj_file_);
		ASSERT_TRUE(obj_ptr_);
		EXPECT_EQ("Apple.obj", obj_ptr_->GetName());
		auto mesh = obj_ptr_->GetMeshes();
		// At least one mesh is present.
		EXPECT_LT(0, mesh.size());
	}

} // End namespace test.
