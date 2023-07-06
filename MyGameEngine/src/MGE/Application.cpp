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


		float vertices[6 * 3] = {
		    -0.25f, -0.25f, 0.0f,  // Triangle 1: Vertex 1
		     0.25f, -0.25f, 0.0f,  // Triangle 1: Vertex 2
		     0.0f,  0.25f, 0.0f,  // Triangle 1: Vertex 3
		     0.0f, -0.75f, 0.0f,  // Triangle 2: Vertex 1
		     -0.25f, -0.25f, 0.0f,  // Triangle 2: Vertex 2
		     0.25f,  -0.25f, 0.0f   // Triangle 2: Vertex 3
		};

		// upload the vertices to GPU to the currently bound VBO and specified as static draw data
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		glEnableVertexAttribArray(0); // This enables the attribute at index 0 to be passed to the vertex shader during rendering.
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		/*
			The code generates an Index Buffer Object (IBO) using glGenBuffers. 
			An IBO is a buffer that stores the indices of the vertices that make up the primitive shapes.
		*/
		// glGenBuffers(2, &m_IndexBuffer);
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer); //  set it as the active buffer for storing index data.

		unsigned int indices[6] = { 0, 1, 2, 3, 4, 5}; // represent the order in which the vertices should be connected to form triangles
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));


		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
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

			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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