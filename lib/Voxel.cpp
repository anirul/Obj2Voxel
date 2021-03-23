#include "Voxel.h"

namespace {
	std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
	{
		os << "glm::vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return os;
	}
}

namespace voxel {

	Voxel::Voxel(const voxel::Obj& obj, const glm::vec3& size)
	{
		LoadPoints(obj);
		voxel_proto_.set_name(obj.GetName());
		// I added 10 cm of wiggle room just in case.
		voxel_proto_.set_begin_x(min_.x - 10.0f);
		voxel_proto_.set_begin_y(min_.y - 10.0f);
		voxel_proto_.set_begin_z(min_.z - 10.0f);
		voxel_proto_.set_end_x(max_.x + 10.0f);
		voxel_proto_.set_end_y(max_.y + 10.0f);
		voxel_proto_.set_end_z(max_.z + 10.0f);
		voxel_proto_.set_size_x((int)size.x);
		voxel_proto_.set_size_y((int)size.y);
		voxel_proto_.set_size_z((int)size.z);
		// Setup the whole vector to 0.
		std::size_t vec_size = (int)size.x * (int)size.y * (int)size.z;
		std::cout << "size: " << vec_size << std::endl;
		voxel_vec_.assign(vec_size, 0.0f);
		TraceFromBorder();
		std::cout << "inner points: " << inner_points_.size() << std::endl;
		TraceInside();
		*voxel_proto_.mutable_data() = { voxel_vec_.begin(), voxel_vec_.end() };
	}

	glm::vec3 Voxel::ToRealPos(const glm::vec3& pos) const
	{
		const float dx = 
			(voxel_proto_.begin_x() - voxel_proto_.end_x()) / 
			(voxel_proto_.size_x() - 1);
		const float dy = 
			(voxel_proto_.begin_y() - voxel_proto_.end_y()) /
			(voxel_proto_.size_y() - 1);
		const float dz = 
			(voxel_proto_.begin_z() - voxel_proto_.end_z()) /
			(voxel_proto_.size_z() - 1);
		return
			{
				voxel_proto_.begin_x() + dx * (float)pos.x,
				voxel_proto_.begin_y() + dy * (float)pos.y,
				voxel_proto_.begin_z() + dz * (float)pos.z
			};
	}

	int Voxel::LinearPos(const glm::vec3& pos) const
	{
		return
			static_cast<int>(
				pos.x +
				pos.y * (float)voxel_proto_.size_x() +
				pos.z * (float)voxel_proto_.size_y() * 
					(float)voxel_proto_.size_x());
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
					ComputePoint(x, y, z);
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
					ComputePoint(x, y, z);
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
					ComputePoint(x, y, z);
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
					ComputeInnerPoint(x, y, z);
		}
		std::cout << "\n";
	}

	std::pair<float, glm::vec3> Voxel::GetMinDistance(const glm::vec3& vec)
	{
		float min = std::numeric_limits<float>::max();
		glm::vec3 point;
		#pragma omp parallel for
		for (int i = 0; i < points_.size(); ++i)
		{
			const auto& p = points_[i];
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
		#pragma omp parallel for
		for (int i = 0; i < inner_points_.size(); ++i)
		{
			const auto& p = inner_points_[i];
			float l = glm::length(p - vec);
			min = std::min(min, l);
		}
		return min;
	}

	void Voxel::ComputePoint(int x, int y, int z)
	{
		glm::vec3 pos = { x, y, z };
		int linear = LinearPos(pos);
		glm::vec3 real_pos = ToRealPos(pos);
		auto f_vec = GetMinDistance(real_pos);
		voxel_vec_[linear] = f_vec.first;
		inner_points_.push_back(f_vec.second);
	}

	void Voxel::ComputeInnerPoint(int x, int y, int z)
	{
		glm::vec3 pos = { x, y, z };
		int linear = LinearPos(pos);
		glm::vec3 real_pos = ToRealPos(pos);
		auto f = GetMinInnerDistance(real_pos);
		voxel_vec_[linear] = f;
	}

} // End namespace voxel.
