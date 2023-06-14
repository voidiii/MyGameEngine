#include "MGEpch.h"
#include "WindowsWindow.h"
#include "Log.h"
#include "Core.h"

namespace MGE {

	static bool g_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}

	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props) {
		m_Data.Title = props.Title;
		m_Data.Height = props.Height;
		m_Data.Width = props.Width;

		MGE_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!g_GLFWInitialized) {
			int success = glfwInit();
			HZ_CORE_ASSERT(success, "Could not intialize GLFW!");

			g_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}

	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate() {

		// Check if there are any pending events and process them.
		// Swap the front and back buffers (update the window with the new frame).

		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enable) {

		// If enable is true, sets the swap interval for the current context, i.e., the number of screen updates
		// to wait from the time glfwSwapBuffers was called before swapping the buffers and returning. This is used
		// to synchronize the swapping with the vertical blank (VSync).

		if (enable) {
			glfwSwapInterval(1);
		} else {
			glfwSwapInterval(0);
		}
		m_Data.VSync = enable;
	}

	bool WindowsWindow::IsVSync() const {
		return m_Data.VSync;
	}

}
