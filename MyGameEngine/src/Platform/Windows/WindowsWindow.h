#pragma once
#include "Window.h"

#include <GLFW/glfw3.h>

namespace MGE {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallBackFunction& callback) override { m_Data.EventCallBack = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Height;
			unsigned int Width;

			// Vertical Synchronization, or VSync, is a graphics technology that synchronizes the frame rate of 
			// a game to the refresh rate of a gaming monitor or screen.
			bool VSync;

			EventCallBackFunction EventCallBack;
		};

		WindowData m_Data;
	};

}



