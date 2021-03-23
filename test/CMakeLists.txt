add_executable(VoxelTest
	main.cpp
	VoxelTest.cpp
	VoxelTest.h
	ObjTest.cpp
	ObjTest.h
)

target_include_directories(VoxelTest
  PUBLIC
	${CMAKE_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(VoxelTest
  PUBLIC
	VoxelLib
	VoxelProtoLib
	GTest::gtest
	GTest::gtest_main
)
    