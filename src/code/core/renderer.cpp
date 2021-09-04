#include "mypch.h"
#include "core/renderer.h"
#include "core/application.h"
#include "core/input.h"
#include "core/engineInfo.h"
#include "core/other/editorCamera.h"

unsigned int Renderer::matrixUBO;
glm::mat4 Renderer::projectionMatrix;
glm::mat4 Renderer::viewMatrix;
glm::mat4 Renderer::viewProjectionMatrix;
glm::vec3 Renderer::clearColor = {0.1f, 0.1f, 0.1f};
glm::vec3 Renderer::viewPos;

#ifndef RELEASE_BUILD
DeferredFrameBuffer Renderer::multisampledFrameBuffer;
ForwardFrameBuffer Renderer::forwardFrameBuffer;
#endif


void Renderer::GlfwErrorCallback(int error_code, const char* error_message) {
    Error("Error Number: " << error_code << " Error Message: " << error_message);
}

int Renderer::InitOpenGL() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    glfwSetErrorCallback(GlfwErrorCallback);

    Application::Get().window = glfwCreateWindow(EngineInfo::SCREEN_WIDTH, EngineInfo::SCREEN_HEIGHT, EngineInfo::name.c_str(), NULL, NULL);
    if (!Application::GetWindow()) {
        Error("Failed to create a GLFW window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(Application::GetWindow());
    glfwShowWindow(Application::GetWindow());

    glfwSetKeyCallback(Application::GetWindow(), Input::KeyInputCallback);
    glfwSetFramebufferSizeCallback(Application::GetWindow(), ResizeCallback);
    glfwSetCursorPosCallback(Application::GetWindow(), Input::MouseCallback);
    glfwSetMouseButtonCallback(Application::GetWindow(), Input::MouseButtonCallback);

    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
        Error("Failed to initialize GLAD");
        return -1;
    }

    glViewport(0, 0, EngineInfo::SCREEN_WIDTH, EngineInfo::SCREEN_HEIGHT);
    glfwSetInputMode(Application::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_DEBUG_OUTPUT);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glfwSwapInterval(EngineInfo::swapInterval);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    return 0;
}

void Renderer::Init() {
    projectionMatrix = glm::perspective(glm::radians(90.0f), (float)EngineInfo::SCREEN_WIDTH / (float)EngineInfo::SCREEN_HEIGHT, 0.1f, 1000.0f);


    glGenBuffers(1, &matrixUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, matrixUBO);



#ifndef RELEASE_BUILD
    multisampledFrameBuffer = DeferredFrameBuffer((float)EngineInfo::SCREEN_WIDTH, (float)EngineInfo::SCREEN_HEIGHT, 0);
    forwardFrameBuffer = ForwardFrameBuffer((float)EngineInfo::SCREEN_WIDTH, (float)EngineInfo::SCREEN_HEIGHT);
#endif
}

void Renderer::SetupMatrices() {

#ifndef RELEASE_BUILD
    if (!Application::isRunning) {
        viewMatrix = Application::editorCamera.GetViewMatrix();
    }
#endif
    viewProjectionMatrix = projectionMatrix * viewMatrix;

    glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projectionMatrix));
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, matrixUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::ShutdownOpenGL() {
    glfwTerminate();
}


void Renderer::ResizeCallback(GLFWwindow * window, int width, int height){
#ifdef RELEASE_BUILD
    SetWindowSize(window, width, height);
#endif

}

void Renderer::SetWindowSize(GLFWwindow * window, int width, int height) {
    if ((EngineInfo::SCREEN_WIDTH != width) || (EngineInfo::SCREEN_HEIGHT != height)) {

        EngineInfo::Resize(width, height);

#ifndef RELEASE_BUILD
        multisampledFrameBuffer.Resize(width, height);
        forwardFrameBuffer.Resize(width, height);
#endif

        float ratio = 1.0f;
        if (EngineInfo::SCREEN_HEIGHT != 0.0f) {
            ratio = (float)EngineInfo::SCREEN_WIDTH / (float)EngineInfo::SCREEN_HEIGHT;
        }
        projectionMatrix = glm::perspective(glm::radians(90.0f), ratio, 0.05f, 1000.0f);
        glViewport(0, 0, EngineInfo::SCREEN_WIDTH, EngineInfo::SCREEN_HEIGHT);
    }
}

void Renderer::StartFrame() {
    //--------------------------Draw----------------------
#ifndef RELEASE_BUILD
    forwardFrameBuffer.Bind();
#endif
    //--------------------------Draw--------------------------
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //--------------------------Draw----------------------
#ifndef RELEASE_BUILD
    multisampledFrameBuffer.Bind();
#endif
    //--------------------------Draw--------------------------
#ifndef RELEASE_BUILD
	/*
    if (Application::isRunning) {
        glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
    }
    else {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    }
	*/
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
#else
    glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
#endif

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Renderer::EndFrame() {
#ifndef RELEASE_BUILD
    multisampledFrameBuffer.Unbind();
#endif
}


