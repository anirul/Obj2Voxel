#pragma once
// This is just an helpfull header that include all the protos.

#include <fstream>
// This is there to avoid most of the warnings.
#if defined(_WIN32) || defined(_WIN64)
#pragma warning(push)
#pragma warning(disable: 4005)
#pragma warning(disable: 4244)
#pragma warning(disable: 4251)
#pragma warning(disable: 4996)
#endif
#include "voxel.pb.h"
// Include the json parser.
#include <google/protobuf/util/json_util.h>
#if defined(_WIN32) || defined(_WIN64)
#pragma warning(pop)
#endif
