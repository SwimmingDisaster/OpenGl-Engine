#include "mypch.h"

#include "shader.h"
#include "camera.h"
#include "lights.h"
#include "texture.h"
#include "input.h"

#include "game.h"

#include "mesh_new.h"
#include "model.h"




float lastX = (float)Game::SCREEN_WIDTH / 2, lastY = (float)Game::SCREEN_HEIGHT / 2;

bool firstMouse = true;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);



int main() {
	std::freopen("other/output.txt", "w", stdout);

	Game game;
	int returnValue = game.Init();//Inits opengl and imgui
	if (returnValue != 0) {
		std::cout << returnValue << std::endl;
		return returnValue;
	}

	game.Start();//inits game elements

	game.Run(); //runs the game

	game.Shutdown();//cleanup of classes and opengl and imgui shutdown

	return 0;
}




void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	Game::SCREEN_WIDTH = width;
	Game::SCREEN_HEIGHT = height;

	Game::projectionMatrix = glm::perspective(glm::radians(90.0f), (float)Game::SCREEN_WIDTH / (float)Game::SCREEN_HEIGHT, 0.1f, 1000.0f);
	glViewport(0, 0, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT);
}


void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}


	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;

	lastX = (float)xpos;
	lastY = (float)ypos;

	Game::mainCamera.ProcessMouseMovement(xoffset, yoffset);
}

