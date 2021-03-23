#pragma once

#include <string>

namespace voxel {

	bool IsFileExist(const std::string& file);
	bool IsDirectoryExist(const std::string& file);
	const std::string FindPath(const std::string& file);

} // End namespace voxel.
