#pragma once

#include "Core.h"
#include "Event/Event.h"
#include "Window.h"

namespace MGE {

	class MGE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// To be defined in CLINENT
	Application* CreateApplication();

}

