#include "mypch.h"

#include "shader.h"
#include "camera.h"
#include "lights.h"
#include "texture.h"
#include "particles.h"

#include "game.h"

#include "mesh_new.h"
#include "model.h"




float lastX = (float)Game::SCREEN_WIDTH / 2, lastY = (float)Game::SCREEN_HEIGHT / 2;

bool firstMouse = true;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);




int main() {
	PROFILE_BEGIN_SESSION("MAIN", "other/profile.json");
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

	PROFILE_END_SESSION();
	return 0;
}




void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	PROFILE_FUNCTION();
	Game::SCREEN_WIDTH = width;
	Game::SCREEN_HEIGHT = height;

	Game::projectionMatrix = glm::perspective(glm::radians(90.0f), (float)Game::SCREEN_WIDTH / (float)Game::SCREEN_HEIGHT, 0.1f, 1000.0f);
	glViewport(0, 0, Game::SCREEN_WIDTH, Game::SCREEN_HEIGHT);
}



void processInput(GLFWwindow * window)
{
	PROFILE_FUNCTION();
	float speedup = 1.0f;


	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		speedup = 3.0f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		Game::mainCamera.ProcessKeyboard(FORWARD,  Game::deltaTime * speedup);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		Game::mainCamera.ProcessKeyboard(BACKWARD,  Game::deltaTime * speedup);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		Game::mainCamera.ProcessKeyboard(LEFT,  Game::deltaTime * speedup);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		Game::mainCamera.ProcessKeyboard(RIGHT, Game::deltaTime * speedup);
	}
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	PROFILE_FUNCTION();

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
			Game::mainCamera.isLocked = false;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else {
			firstMouse = true;
			Game::mainCamera.isLocked = true;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	PROFILE_FUNCTION();
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

