
add_executable(Obj2Voxel 
    main.cpp
)

target_include_directories(Obj2Voxel 
  PUBLIC 
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(Obj2Voxel
  PUBLIC
    VoxelLib
    VoxelProtoLib
    spdlog::spdlog
)

