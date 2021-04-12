#include "mypch.h"



class Renderer {
public:
	static unsigned int matrixUBO;
	static glm::mat4 projectionMatrix;
	static glm::mat4 viewMatrix;
	static glm::mat4 viewProjectionMatrix;
	static glm::vec3 clearColor;

public:
	static int InitOpenGL();
	static void SetupMatrices();
	static void InitMatrices();
	static void ShutdownOpenGL();
	static void ResizeCallback(GLFWwindow * window, int width, int height);
	static void SetWindowSize(GLFWwindow * window, int width, int height);
};