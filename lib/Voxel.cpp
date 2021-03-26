#include "Voxel.h"

namespace {
	std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
	{
		os << "glm::vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const glm::ivec3& pos)
	{
		os << "glm::ivec3(" << pos.x << ", " << pos.y << ", " << pos.z << ")";
		return os;
	}
}

namespace voxel {

	Voxel::Voxel(const voxel::Obj& obj, const glm::ivec3& size)
	{
		LoadPoints(obj);
		voxel_proto_.set_name(obj.GetName());
		// I added 10 cm of wiggle room just in case.
		voxel_proto_.set_begin_x(min_.x - 10.0f);
		voxel_proto_.set_begin_y(min_.y - 10.0f);
		voxel_proto_.set_begin_z(min_.z - 10.0f);
		begin_ = { min_.x - 10.0f, min_.y - 10.0f, min_.z - 10.0f };
		voxel_proto_.set_end_x(max_.x + 10.0f);
		voxel_proto_.set_end_y(max_.y + 10.0f);
		voxel_proto_.set_end_z(max_.z + 10.0f);
		end_ = { max_.x + 10.0f, max_.y + 10.0f, max_.z + 10.0f };
		assert(glm::all(glm::lessThan(begin_, end_)));
		voxel_proto_.set_size_x(size.x);
		voxel_proto_.set_size_y(size.y);
		voxel_proto_.set_size_z(size.z);
		// Setup the whole vector to 0.
		std::size_t vec_size = 
			static_cast<std::size_t>(
				(std::int64_t)size.x * 
				(std::int64_t)size.y * 
				(std::int64_t)size.z);
		std::cout << "size: " << vec_size << std::endl;
		voxel_vec_.assign(vec_size, std::numeric_limits<float>::max());
		TraceFromBorder();
		std::cout << "inner points: " << inner_points_.size() << std::endl;
		TraceInside();
		*voxel_proto_.mutable_data() = { voxel_vec_.begin(), voxel_vec_.end() };
	}

	glm::vec3 Voxel::ToRealPos(const glm::ivec3& pos) const
	{
		const float dx = 
			fabs(voxel_proto_.begin_x() - voxel_proto_.end_x()) / 
			(voxel_proto_.size_x() - 1);
		const float dy = 
			fabs(voxel_proto_.begin_y() - voxel_proto_.end_y()) /
			(voxel_proto_.size_y() - 1);
		const float dz = 
			fabs(voxel_proto_.begin_z() - voxel_proto_.end_z()) /
			(voxel_proto_.size_z() - 1);
		return
			{
				voxel_proto_.begin_x() + dx * (float)pos.x,
				voxel_proto_.begin_y() + dy * (float)pos.y,
				voxel_proto_.begin_z() + dz * (float)pos.z
			};
	}

	int Voxel::LinearPos(const glm::ivec3& pos) const
	{
		return	pos.x +
				pos.y * voxel_proto_.size_x() +
				pos.z * voxel_proto_.size_y() * voxel_proto_.size_x();
	}

	void Voxel::LoadPoints(const voxel::Obj& obj)
	{
		const auto& meshes = obj.GetMeshes();
		std::cout << "Found #" << meshes.size() << " meshes.\n";
		std::int64_t total_vertices = 0;
		bool initialized = false;
		for (const auto& mesh : meshes)
		{
			const auto& vertices = mesh.GetVertices();
			std::cout << "\tmesh has #" << vertices.size() << " vertices.\n";
			total_vertices += vertices.size();
			if (!initialized)
			{
				min_ = vertices[0].point;
				max_ = vertices[0].point;
				initialized = true;
			}
			for (const auto& element : vertices)
			{
				glm::vec3 temp = element.point;
				min_ = glm::min(min_, temp);
				max_ = glm::max(max_, temp);
				points_.push_back(temp);
			}
		}
		std::cout << "Box: " << glm::abs(max_ - min_) << "\n";
	}

	void Voxel::TraceFromBorder()
	{
		// x
		for (int x = 0; 
			x < voxel_proto_.size_x(); 
			x += voxel_proto_.size_x() - 1)
		{
			for (int y = 0; y < voxel_proto_.size_y(); ++y)
			{
				std::cout << ".";
				for (int z = 0; z < voxel_proto_.size_z(); ++z)
					ComputePoint({ x, y, z });
			}
		}
		// y
		for (int x = 0; x < voxel_proto_.size_z(); ++x)
		{
			for (int y = 0; 
				y < voxel_proto_.size_y(); 
				y += voxel_proto_.size_y() - 1)
			{
				std::cout << ".";
				for (int z = 0; z < voxel_proto_.size_z(); ++z)
					ComputePoint({ x, y, z });
			}
		}
		// z
		for (int x = 0; x < voxel_proto_.size_z(); ++x)
		{
			std::cout << ".";
			for (int y = 0; y < voxel_proto_.size_y(); ++y)
			{
				for (int z = 0;
					z < voxel_proto_.size_z();
					z += voxel_proto_.size_z() - 1)
					ComputePoint({ x, y, z });
			}
		}
		std::cout << "\n";
	}

	void Voxel::TraceInside()
	{
		for (int x = 1; x < voxel_proto_.size_x() - 1; ++x)
		{
			std::cout << ".";
			for (int y = 1; y < voxel_proto_.size_y() - 1; ++y)
				for (int z = 1; z < voxel_proto_.size_z() - 1; ++z)
					ComputeInnerPoint({ x, y, z });
		}
		std::cout << "\n";
	}

	std::pair<float, glm::vec3> Voxel::GetMinDistance(const glm::vec3& vec)
	{
		float min = std::numeric_limits<float>::max();
		glm::vec3 point;
		for (const auto p : points_)
		{
			float l = glm::length(p - vec);
			min = std::min(min, l);
			if (min == l)
			{
				point = p;
			}
		}
		return { min, point };
	}

	float Voxel::GetMinInnerDistance(const glm::vec3& vec)
	{
		float min = std::numeric_limits<float>::max();
		for (const glm::vec3& v3 : inner_points_)
		{
			float l = glm::length(v3 - vec);
			min = std::min(min, l);
		}
		return min;
	}

	void Voxel::ComputePoint(const glm::ivec3& pos)
	{
		int linear = LinearPos(pos);
		glm::vec3 real_pos = ToRealPos(pos);
		auto f_vec = GetMinDistance(real_pos);
		voxel_vec_[linear] = std::min(f_vec.first, voxel_vec_[linear]);
		inner_points_.push_back(f_vec.second);
	}

	void Voxel::ComputeInnerPoint(const glm::ivec3& pos)
	{
		int linear = LinearPos(pos);
		glm::vec3 real_pos = ToRealPos(pos);
		assert(glm::all(glm::greaterThanEqual(real_pos, begin_)));
		assert(glm::all(glm::lessThan(real_pos, end_)));
		auto f = GetMinInnerDistance(real_pos);
		voxel_vec_[linear] = std::min(f, voxel_vec_[linear]);
	}

} // End namespace voxel.
