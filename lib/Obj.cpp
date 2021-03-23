#include "lib/Obj.h"
#include <fstream>
#include <iostream>
#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include <tiny_obj_loader.h>
#include <fmt/core.h>

namespace voxel {

	Obj::Obj(const std::string& file_path, const std::string& file_name)
	{
		name_ = file_name;
		tinyobj::ObjReaderConfig reader_config;
		reader_config.mtl_search_path = file_path;
		tinyobj::ObjReader reader;
		if (!reader.ParseFromFile(file_path + file_name))
		{
			if (!reader.Error().empty())
			{
				throw std::runtime_error(reader.Error());
			}
			throw std::runtime_error(
				fmt::format(
					"Unknown error parsing file {}/{}",
					file_path,
					file_name));
		}
		
		std::cout << 
			fmt::format(
				"File {}/{} oppened and parsed.\n", 
				file_path, 
				file_name);
		if (!reader.Warning().empty())
		{
			std::cerr << fmt::format(
				"Warning parsing file {}: {}\n", 
				file_name, 
				reader.Warning());
		}

		auto& attrib = reader.GetAttrib();
		auto& shapes = reader.GetShapes();
		auto& materials = reader.GetMaterials();

		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) 
		{
			std::vector<ObjVertex> points; 
			std::vector<int> indices; 

			// Loop over faces(polygon) this should be triangles?
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) 
			{
				int fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					ObjVertex vertex{};
					// access to vertex
					tinyobj::index_t idx = 
						shapes[s].mesh.indices[index_offset + v];
					vertex.point.x = attrib.vertices[3 * idx.vertex_index + 0];
					vertex.point.y = attrib.vertices[3 * idx.vertex_index + 1];
					vertex.point.z = attrib.vertices[3 * idx.vertex_index + 2];
					vertex.normal.x = attrib.normals[3 * idx.normal_index + 0];
					vertex.normal.y = attrib.normals[3 * idx.normal_index + 1];
					vertex.normal.z = attrib.normals[3 * idx.normal_index + 2];
					if (idx.texcoord_index != -1)
					{
						vertex.tex_coord.x = 
							attrib.texcoords[2 * idx.texcoord_index + 0];
						vertex.tex_coord.y = 
							attrib.texcoords[2 * idx.texcoord_index + 1];
					}
					points.push_back(vertex);
				}
				index_offset += fv;

				// per-face material				
				// shapes[s].mesh.material_ids[f];
			}
			ObjMesh mesh(points, indices, -1);
			meshes_.push_back(mesh);
		}
	}

} // End namespace voxel.
