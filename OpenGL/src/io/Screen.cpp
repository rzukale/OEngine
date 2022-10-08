#include "Screen.h"
#include "Keyboard.h"
#include "Mouse.h"

unsigned int Screen::SCREEN_WIDTH = 1280;
unsigned int Screen::SCREEN_HEIGHT = 720;

Screen::Screen() : m_Window(nullptr) { }

Screen::~Screen()
{
}

void Screen::FrameBufferSizeCallback(GLFWwindow* Window, int Width, int Height)
{
    GLCall(glViewport(0, 0, Width, Height));

    SCREEN_WIDTH = Width;
    SCREEN_HEIGHT = Height;
}

bool Screen::Init()
{
    m_Window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL", NULL, NULL);
    if (!m_Window)
        return false;

    glfwMakeContextCurrent(m_Window);
    return true;
}

void Screen::SetParameters()
{
    GLCall(glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

    glfwSetFramebufferSizeCallback(m_Window, Screen::FrameBufferSizeCallback);

    glfwSetKeyCallback(m_Window, Keyboard::KeyCallback);
    glfwSetMouseButtonCallback(m_Window, Mouse::MouseButtonCallback);
    glfwSetCursorPosCallback(m_Window, Mouse::CursorPositionCallback);
    glfwSetScrollCallback(m_Window, Mouse::MouseWheelCallback);

    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Screen::Update() // should be clear
{
    GLCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Screen::NewFrame()
{
    GLCall(glfwSwapBuffers(m_Window));
    GLCall(glfwPollEvents());
}

bool Screen::ShouldClose()
{
    return glfwWindowShouldClose(m_Window);
}

void Screen::SetShouldClose(bool bShouldClose)
{
    glfwSetWindowShouldClose(m_Window, bShouldClose);
}
