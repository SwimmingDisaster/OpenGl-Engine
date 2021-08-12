#include "mypch.h"
#include "core/framebuffer.h"

class Renderer
{
public:
	static unsigned int matrixUBO;
	static glm::mat4 projectionMatrix;
	static glm::mat4 viewMatrix;
	static glm::mat4 viewProjectionMatrix;
	static glm::vec3 clearColor;

#ifndef RELEASE_BUILD
	static DeferredFrameBuffer multisampledFrameBuffer;
#endif

public:
	static int InitOpenGL();
	static void SetupMatrices();
	static void Init();
	static void ShutdownOpenGL();
	static void ResizeCallback(GLFWwindow *window, int width, int height);
	static void SetWindowSize(GLFWwindow *window, int width, int height);
	
	static void GlfwErrorCallback(int error_code, const char* error_message);

	static void StartFrame();
	static void EndFrame();
};
