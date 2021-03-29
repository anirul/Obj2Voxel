#pragma once

#include <set>
#include "lib/Obj.h"
#include "proto/proto.h"

namespace {

	struct CompareGlmVec
	{
		bool operator()(const glm::vec3& l, const glm::vec3& r) const
		{
			// Use tuple because they have a correct < operator.
			std::tuple lt(l.x, l.y, l.z);
			std::tuple rt(r.x, r.y, r.z);
			return lt < rt;
		}
	};

} // End namespace.

namespace voxel {

	class Voxel 
	{
	public:
		Voxel(const Obj& obj, const glm::ivec3& size);
		const proto::VoxelFile& GetVoxelFile() const
		{ 
			return voxel_proto_; 
		}
		int LinearPos(const glm::ivec3& pos) const;

	protected:
		glm::ivec3 GetClosestSide(const glm::ivec3& pos) const;
		glm::vec3 ToRealPos(const glm::ivec3& pos) const;
		void LoadPoints(const voxel::Obj& obj);
		void TraceFromBorder();
		void TraceInside();
		std::pair<float, glm::vec3> GetMinDistance(const glm::vec3& vec);
		float GetMinInnerDistance(const glm::vec3& vec, const glm::ivec3& pos);
		void ComputePoint(const glm::ivec3& pos);
		void ComputeInnerPoint(const glm::ivec3& pos);

	private:
		proto::VoxelFile voxel_proto_ = {};
		glm::vec3 min_;
		glm::vec3 max_;
		glm::vec3 begin_;
		glm::vec3 end_;
		std::vector<glm::vec3> points_;
		std::set<glm::vec3, CompareGlmVec> inner_points_;
		std::vector<float> voxel_vec_;
	};

} // End namespace voxel.
