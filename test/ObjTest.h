#pragma once

#include <gtest/gtest.h>
#include "lib/FileSystem.h"
#include "lib/Obj.h"

namespace test {

	class ObjTest : public testing::Test 
	{
	public:
		ObjTest() = default;

	protected:
		const std::string path_ = voxel::FindPath("asset/models/");
		const std::string obj_file_ = "Apple.obj";
		std::unique_ptr<voxel::Obj> obj_ptr_ = nullptr;
	};

} // End namespace test.
