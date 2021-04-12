add_library(VoxelLib
  STATIC
	FileSystem.cpp
	FileSystem.h
    Nrrd.cpp
	Nrrd.h
	Obj.cpp
    Obj.h
	Voxel.cpp
	Voxel.h
)

target_include_directories(VoxelLib
  PUBLIC
	${CMAKE_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(VoxelLib
  PUBLIC
	fmt::fmt
    glm
	tinyobjloader::tinyobjloader
)
