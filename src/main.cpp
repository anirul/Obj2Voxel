#include <algorithm>
#include <filesystem>
#include <iostream>
#include <cassert>
#include <limits>
#include <set>
#include "lib/Obj.h"
#include "lib/Voxel.h"
#include "proto/proto.h"

bool IsFileExist(const std::string& file)
{
	return std::filesystem::is_regular_file(file);
}

bool IsDirectoryExist(const std::string& file)
{
	return std::filesystem::is_directory(file);
}

const std::string FindPath(const std::string& file)
{
	// This is a bad hack as this won't prevent people from adding Asset
	// high in the path of the game.
	for (auto i : { 0, 4, 3, 2, 1 })
	{
		std::string f;
		for (auto j = 0; j < i; ++j)
			f += "../";
		f += file;
		if (IsDirectoryExist(f))
		{
			std::filesystem::path p(f);
			return std::filesystem::absolute(p).string();
		}
	}
	return "";
}

int main(int ac, char** av)
{
    try 
    {
        assert(ac == 3);
		voxel::Obj obj(FindPath(av[1]), av[2]);
		voxel::Voxel vox(obj, { 16.f, 16.f, 16.f });
		SaveProtoToJsonFile(vox.GetVoxelFile(), "./test.json");
    } 
    catch(std::exception& ex) 
    {
        std::cout << "std::exception: " << ex.what() << "\n";
    }
    return 0;
}
