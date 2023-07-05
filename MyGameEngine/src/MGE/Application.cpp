#include "MGEpch.h"
#include "Application.h"
#include "./Event/ApplicationEvent.h"
#include "./Log.h"

#include "Input.h"

#include <glad/glad.h>

// This creates a function object that, when called (the whole macro would be a function), will invoke x on this Application object.
#define BIND_EVENT_FUNCTION(x) std::bind(&Application::x, this, std::placeholders::_1)

namespace MGE {

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		MGE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FUNCTION(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		/*
			The code generates a Vertex Array Object (VAO) using glGenVertexArrays. 
			A VAO is an object that stores the configurations of vertex attributes 
			(such as position, color, etc.) used for rendering. 
			It acts as a container for vertex buffer bindings and attribute configurations.
		*/
		glGenVertexArrays(2, &m_VertexArray);
		glBindVertexArray(m_VertexArray); // The VAO is then bound using glBindVertexArray to set it as the active VAO.

		/*
			The code generates a Vertex Buffer Object (VBO) using glGenBuffers. 
			A VBO is a buffer that holds vertex data, such as positions, colors, or texture coordinates. 
			In this case, it is used to store the vertices of the triangle.
		*/
		glGenBuffers(2, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

		float vertices[6 * 3] = {
		    -0.25f, -0.25f, 0.0f,  // Triangle 1: Vertex 1
		     0.25f, -0.25f, 0.0f,  // Triangle 1: Vertex 2
		     0.0f,  0.25f, 0.0f,  // Triangle 1: Vertex 3
		     0.0f, -0.75f, 0.0f,  // Triangle 2: Vertex 1
		     -0.25f, -0.25f, 0.0f,  // Triangle 2: Vertex 2
		     0.25f,  -0.25f, 0.0f   // Triangle 2: Vertex 3
		};

		// upload the vertices to GPU to the currently bound VBO and specified as static draw data
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0); // This enables the attribute at index 0 to be passed to the vertex shader during rendering.
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		/*
			The code generates an Index Buffer Object (IBO) using glGenBuffers. 
			An IBO is a buffer that stores the indices of the vertices that make up the primitive shapes.
		*/
		glGenBuffers(2, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer); //  set it as the active buffer for storing index data.

		unsigned int indices[6] = { 0, 1, 2, 3, 4, 5}; // represent the order in which the vertices should be connected to form triangles
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	Application::~Application() {

	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);

		// Dispatch function will compare the e from dispatcher and the WindowCloseEvent
		// if they are the same type, the macro would be called and thus invoke the OnWindowClose function of this Application
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNCTION(OnWindowClose));
		
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
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

			for (auto layer : m_LayerStack) {
				layer->OnUpdate();
			}
			
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();
			
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
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