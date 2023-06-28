#pragma once

#include "Core.h"
#include "Event/Event.h"
#include "Window.h"
#include "Event/ApplicationEvent.h"
#include "Event/MouseEvent.h"
#include "Event/KeyEvent.h"
#include "LayerStack.h"

namespace MGE {

	class MGE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		/*
			A static member function can be thought of as a global function that belongs to a class's scope and
			can access the private and protected members of the class. It doesn't require an instance of the class to be called, 
			but it can only directly access other static members (variables or functions) of the class.
		*/
		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

	private:
		static Application* s_Instance;
	};

	// To be defined in CLINENT
	Application* CreateApplication();

}

