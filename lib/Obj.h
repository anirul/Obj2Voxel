#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace voxel {

	struct ObjVertex 
	{
		glm::vec3 point;
		glm::vec3 normal;
		glm::vec2 tex_coord;
	};

	class ObjMesh 
	{
	public:
		ObjMesh(
			std::vector<ObjVertex> points, 
			std::vector<int> indices, 
			int material) : 
			points_(points), indices_(indices), material_(material) {}
		const std::vector<ObjVertex>& GetVertices() const { return points_; }
		const std::vector<int>& GetIndices() const { return indices_; }
		int GetMaterialIds() const { return material_; }

	protected:
		std::vector<ObjVertex> points_ = {};
		std::vector<int> indices_ = {};
		int material_ = -1;
	};

	class ObjMaterial
	{
	public:
		// TODO(anirul): implement me!
	};

	class Obj 
	{
	public:
		// Parse from an OBJ file.
		Obj(const std::string& file_path, const std::string& file_name);
		// Get meshes, they are suppose to be sorted by material.
		const std::vector<ObjMesh> GetMeshes() const { return meshes_; }
		// Get the materials, id in mesh give the material in the vector.
		const std::vector<ObjMaterial> GetMaterials() const 
		{ 
			return materials_; 
		}
		const std::string& GetName() const { return name_; }
	
	protected:
		std::string name_;
		std::vector<ObjMesh> meshes_;
		std::vector<ObjMaterial> materials_;
	};

} // End namespace voxel.
