#include "MGEpch.h"
#include "Application.h"
#include "./Event/ApplicationEvent.h"
#include "./Log.h"

#include "Input.h"
#include "MGE/Renderer/RenderCommand.h"
#include "MGE/Renderer/Renderer.h"

#include <glfw/glfw3.h>
// This creates a function object that, when called (the whole macro would be a function), will invoke x on this Application object.
#define BIND_EVENT_FUNCTION(x) std::bind(&Application::x, this, std::placeholders::_1)

namespace MGE {

	Application* Application::s_Instance = nullptr;

	Application::Application() 
	{
		MGE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FUNCTION(OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		
	}

	Application::~Application() {

	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);

		// Dispatch function will compare the e from dispatcher and the WindowCloseEvent
		// if they are the same type, the macro would be called and thus invoke the OnWindowClose function of this Application
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNCTION(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNCTION(OnWindowResize));
		
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			
			// ensure that only the topmost layer that can handle a particular event will do so, 
			// and once an event has been handled, lower layers will not process it.

			(*--it)->OnEvent(e);
			if (e.Handled) {
				break;
			}
		}
	}

	void Application::Run() {

		while (m_Running) {
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime; // 1.0f / 60.0f;
			timestep.SetFrameRate(time - m_LastFrameTime);
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}
			
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender(timestep);
			m_ImGuiLayer->End();
			
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e) {
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);	
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushOverLayer(layer);	
		layer->OnAttach();
	}


}