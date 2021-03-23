#pragma once

#include <set>
#include "lib/Obj.h"
#include "proto/proto.h"

namespace {
	struct compareVec
	{
		bool operator() (const glm::vec3& lhs, const glm::vec3& rhs) const
		{
			return glm::all(glm::lessThan(lhs, rhs));
		}
	};
}

namespace voxel {

	class Voxel 
	{
	public:
		Voxel(const voxel::Obj& obj, const glm::vec3& size);
		const voxel::proto::VoxelFile& GetVoxelFile() const
		{ 
			return voxel_proto_; 
		}
		int LinearPos(const glm::vec3& pos) const;

	protected:
		glm::vec3 ToRealPos(const glm::vec3& pos) const;
		void LoadPoints(const voxel::Obj& obj);
		void TraceFromBorder();
		void TraceInside();
		std::pair<float, glm::vec3> GetMinDistance(const glm::vec3& vec);
		float GetMinInnerDistance(const glm::vec3& vec);
		void ComputePoint(int x, int y, int z);
		void ComputeInnerPoint(int x, int y, int z);

	private:
		voxel::proto::VoxelFile voxel_proto_ = {};
		glm::vec3 min_;
		glm::vec3 max_;
		std::vector<glm::vec3> points_;
		std::vector<glm::vec3> inner_points_;
		std::vector<float> voxel_vec_;
	};

} // End namespace voxel.
