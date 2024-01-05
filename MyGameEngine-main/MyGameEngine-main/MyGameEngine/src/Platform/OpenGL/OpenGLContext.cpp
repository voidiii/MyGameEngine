#include "MGEpch.h"
#include "OpenGLContext.h"
#include "Core.h"
#include "Log.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>

namespace MGE {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		MGE_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		MGE_CORE_ASSERT(status, "Failed to initialize Glad!");

		MGE_CORE_INFO("OpenGL Info:");
		MGE_CORE_INFO("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
		MGE_CORE_INFO("  Renderer: {}", (char*)glGetString(GL_RENDERER));
		MGE_CORE_INFO("  Version: {}", (char*)glGetString(GL_VERSION));
	}

	/*
		This is necessary for double-buffered rendering, 
		where the back buffer contains the newly rendered frame 
		and the front buffer is displayed on the screen. 
		Swapping the buffers ensures that the newly rendered frame becomes visible.
	*/
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}