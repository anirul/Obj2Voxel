
add_library(VoxelProtoLib
  STATIC
    voxel.proto
    proto.h
)

target_include_directories(VoxelProtoLib 
  PUBLIC 
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

# I still doesn't know how to pass the path correctly to the generate in case
# there is any external include.
protobuf_generate(APPEND_PATH TARGET VoxelProtoLib)

target_link_libraries(VoxelProtoLib 
  PUBLIC
    protobuf::libprotoc
    protobuf::libprotobuf
)
