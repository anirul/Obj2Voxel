add_library(VoxelLib
  STATIC
    Obj.cpp
    Obj.h
)

target_include_directories(VoxelLib
  PUBLIC
	${CMAKE_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(VoxelLib
  PUBLIC
    glm
	tinyobjloader::tinyobjloader
)
