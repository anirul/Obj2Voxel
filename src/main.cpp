#include <algorithm>
#include <filesystem>
#include <iostream>
#include <cassert>
#include <limits>
#include <set>
#include "lib/FileSystem.h"
#include "lib/Obj.h"
#include "lib/Voxel.h"
#include "proto/proto.h"

int main(int ac, char** av)
{
    try 
    {
        assert(ac == 3);
		voxel::Obj obj(voxel::FindPath(av[1]), av[2]);
		voxel::Voxel vox(obj, { 16.f, 16.f, 16.f });
		SaveProtoToJsonFile(vox.GetVoxelFile(), "./test.json");
    } 
    catch(std::exception& ex) 
    {
        std::cout << "std::exception: " << ex.what() << "\n";
    }
    return 0;
}
