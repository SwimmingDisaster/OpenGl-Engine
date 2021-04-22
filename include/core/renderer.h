#include "mypch.h"
#include "frameBuffer.h"



class Renderer {
public:
	static unsigned int matrixUBO;
	static glm::mat4 projectionMatrix;
	static glm::mat4 viewMatrix;
	static glm::mat4 viewProjectionMatrix;
	static glm::vec3 clearColor;

#ifndef RELEASE_BUILD
	static FrameBuffer frameBuffer;
#endif


public:
	static int InitOpenGL();
	static void SetupMatrices();
	static void Init();
	static void ShutdownOpenGL();
	static void ResizeCallback(GLFWwindow * window, int width, int height);
	static void SetWindowSize(GLFWwindow * window, int width, int height);

	static void StartFrame();
	static void EndFrame();

};