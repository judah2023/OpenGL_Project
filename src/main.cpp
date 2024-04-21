#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "context.h"

// 윈도우 프레임버퍼(창) 크기 변경 이벤트 콜백함수
void OnFramebufferSizeChange(GLFWwindow * wnd, int width, int height);
// 키보드 입력 이벤트 콜백
void OnKeyEvent(GLFWwindow* wnd, int key, int scanCode, int action, int mods);
// 미우스 커서 이벤트 콜백
void OnCursorPos(GLFWwindow* wnd, double x, double y);
// 마우스 클릭 이벤트 콜백
void OnMouseButton(GLFWwindow* wnd, int button, int action, int modifier);
// 랜더링 함수
void Render();

int main(int argc, const char** argv) 
{
    // 시작을 알리는 로그
    SPDLOG_INFO("Start program");

    // glfw 라이브러리 초기화, 실패하면 에러 출력후 종료
    SPDLOG_INFO("Initialize glfw");
    if (!glfwInit()) 
    {
        const char* description = nullptr;
        glfwGetError(&description);
        SPDLOG_ERROR("failed to initialize glfw: {}", description);
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw 윈도우 생성, 실패하면 에러 출력후 종료
    SPDLOG_INFO("Create glfw window");
    auto wnd = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME,
      nullptr, nullptr);
    if (!wnd) 
    {
        SPDLOG_ERROR("failed to create glfw window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(wnd);

    // glad를 활용한 OpenGL 함수 로딩
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        SPDLOG_INFO("failed to initialize glad");
        glfwTerminate();
        return -1;
    }
    auto glVersion = glGetString(GL_VERSION);
    SPDLOG_INFO("OpenGL context version : {}", (char*)glVersion);

    auto context = Context::Create();
    if (!context)
    {
        SPDLOG_INFO("failed to create context");
        glfwTerminate();
        return -1;
    }
    glfwSetWindowUserPointer(wnd, context.get());

    OnFramebufferSizeChange(wnd, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(wnd, OnFramebufferSizeChange);
    glfwSetKeyCallback(wnd, OnKeyEvent);
    glfwSetCursorPosCallback(wnd, OnCursorPos);
    glfwSetMouseButtonCallback(wnd, OnMouseButton);

    // glfw 루프 실행, 윈도우 close 버튼을 누르면 정상 종료
    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(wnd)) 
    {
        context->ProcessInput(wnd);
        context->Render();
        glfwSwapBuffers(wnd);
        glfwPollEvents();
    }
    context.reset();

    glfwTerminate();
    return 0;
}

void OnFramebufferSizeChange(GLFWwindow *wnd, int width, int height)
{
    SPDLOG_INFO("framebuffer size changed : ({} x {})", width, height);
    auto context = (Context*) glfwGetWindowUserPointer(wnd);
    context->Reshape(width, height);
}

void OnKeyEvent(GLFWwindow *wnd, int key, int scanCode, int action, int mods)
{
    SPDLOG_INFO("key: {}, scancode: {}, action: {}, mods: {}{}{}",
        key, scanCode,
        action == GLFW_PRESS ? "Pressed" :
        action == GLFW_RELEASE ? "Released" :
        action == GLFW_REPEAT ? "Repeat" : "Unknown",
        mods & GLFW_MOD_CONTROL ? "C" : "-",
        mods & GLFW_MOD_SHIFT ? "S" : "-",
        mods & GLFW_MOD_ALT ? "A" : "-");
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
    {
        glfwSetWindowShouldClose(wnd, true);
    }
}

void OnCursorPos(GLFWwindow *wnd, double x, double y)
{
    auto context = (Context*) glfwGetWindowUserPointer(wnd);
    context->MouseMove(x, y);
}

void OnMouseButton(GLFWwindow *wnd, int button, int action, int modifier)
{
    auto context = (Context*) glfwGetWindowUserPointer(wnd);
    double x, y;
    glfwGetCursorPos(wnd, &x, &y);
    context->MouseButton(button, action, x, y);
}

void Render()
{
    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
