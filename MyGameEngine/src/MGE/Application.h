#pragma once

#include "Core.h"
#include "Event/Event.h"

namespace MGE {

	class MGE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in CLINENT
	Application* CreateApplication();

}

