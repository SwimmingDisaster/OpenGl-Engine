#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

#define GLEW_STATIC
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>
#include <ImGUI/imgui_stdlib.h>


#include "shader.h"
#include "camera.h"
#include "lights.h"
#include "game.h"
#include "texture.h"
#include "particles.h"
#include "lightManager.h"
#include "log.h"
#include "mesh_new.h"
#include "model.h"

#include "profiler.h"





/*extern std::unordered_map<int, bool>  eventHeld;
extern std::unordered_map<int, bool>  eventPressed;
extern std::unordered_map<int, bool>  eventReleased;*/
