#include "mypch.h"



class Renderer {
public:
	static unsigned int matrixUBO;
	static glm::mat4 projectionMatrix;
	static glm::mat4 viewMatrix;
	static glm::mat4 viewProjectionMatrix;


public:
	static int InitOpenGL();
	static void SetupMatrices();
	static void InitMatrices();
	static void ShutdownOpenGL();
};