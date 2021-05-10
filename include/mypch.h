#pragma once
//#pragma warning(disable : 4005)

#define NOMINMAX
#define _DEBUG
//#define SHOW_SCENE_SERIALISATION
//#define SHOW_DELETED
//#define RELEASE_BUILD

#include <iostream>
#include <float.h>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <random>
#include <limits>

#include <string>
#include <sstream>

#include <fstream>
#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>

#define GLEW_STATIC
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>

//#define PX_SUPPORT_GPU_PHYSX 1
#include "physx/PxPhysicsAPI.h"

#define IMGUI_DEFINE_PLACEMENT_NEW
#define IMGUI_DEFINE_MATH_OPERATORS
#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include <ImGUI/imgui_stdlib.h>
#include <ImGUI/imgui_internal.h>

#include <imguizmo/ImGuizmo.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <yaml-cpp/yaml.h>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/detail/node.h>

#include <stb/stb_image.h>

#include "core/other/log.h"
#include "core/other/random.h"

#include "core/other/helpers.h"

#include "core/math.h"

#include <nfd/nfd.hpp>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
//#include <Windows.h>
#endif

using namespace physx; //all classes are prefixed with Px so its very hard to confuze classes from the namespace physx and no namespace :)