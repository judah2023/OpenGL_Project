#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// imgui
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// My headers
#include "context.h"

// 윈도우 프레임버퍼(창) 크기 변경 이벤트 콜백함수
void OnFramebufferSizeChange(GLFWwindow * wnd, int width, int height);
// 키보드 입력 이벤트 콜백
void OnKeyEvent(GLFWwindow* wnd, int key, int scanCode, int action, int mods);
// 문자 입력 이벤트 콜백
void OnCharEvent(GLFWwindow* wnd, UINT ch);
// 미우스 커서 이벤트 콜백
void OnCursorPos(GLFWwindow* wnd, double x, double y);
// 마우스 클릭 이벤트 콜백
void OnMouseButton(GLFWwindow* wnd, int button, int action, int modifier);
// 마우스 스크롤 이벤트 콜백
void OnScroll(GLFWwindow* wnd, double xOffset, double yOffset);
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
    GLFWwindow* wnd = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME,
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

    auto guiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(guiContext);
    ImGui_ImplGlfw_InitForOpenGL(wnd, false);
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplOpenGL3_CreateFontsTexture();
    ImGui_ImplOpenGL3_CreateDeviceObjects();

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
    glfwSetCharCallback(wnd, OnCharEvent);
    glfwSetCursorPosCallback(wnd, OnCursorPos);
    glfwSetMouseButtonCallback(wnd, OnMouseButton);
    glfwSetScrollCallback(wnd, OnScroll);

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // glfw 루프 실행, 윈도우 close 버튼을 누르면 정상 종료
    SPDLOG_INFO("Start main loop");
    while (!glfwWindowShouldClose(wnd)) 
    {
        glfwPollEvents();

        //
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        context->ProcessInput(wnd);
        context->Render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //

        glfwSwapBuffers(wnd);
    }
    context.reset();

    ImGui_ImplOpenGL3_DestroyFontsTexture();
    ImGui_ImplOpenGL3_DestroyDeviceObjects();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(guiContext);

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
    ImGui_ImplGlfw_KeyCallback(wnd, key, scanCode, action, mods);
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

void OnCharEvent(GLFWwindow *wnd, UINT ch)
{
    ImGui_ImplGlfw_CharCallback(wnd, ch);
}

void OnCursorPos(GLFWwindow *wnd, double x, double y)
{
    auto context = (Context*) glfwGetWindowUserPointer(wnd);
    context->MouseMove(x, y);
}

void OnMouseButton(GLFWwindow *wnd, int button, int action, int mod)
{
    ImGui_ImplGlfw_MouseButtonCallback(wnd, button, action, mod);
    auto context = (Context*) glfwGetWindowUserPointer(wnd);
    double x, y;
    glfwGetCursorPos(wnd, &x, &y);
    context->MouseButton(button, action, x, y);
}

void OnScroll(GLFWwindow *wnd, double xOffset, double yOffset)
{
    ImGui_ImplGlfw_ScrollCallback(wnd, xOffset, yOffset);
}

void Render()
{
    glClearColor(0.1f, 0.2f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
