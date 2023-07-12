#include "MGEpch.h"
#include "Application.h"
#include "./Event/ApplicationEvent.h"
#include "./Log.h"

#include "Input.h"
#include "MGE/Renderer/RenderCommand.h"
#include "MGE/Renderer/Renderer.h"
// This creates a function object that, when called (the whole macro would be a function), will invoke x on this Application object.
#define BIND_EVENT_FUNCTION(x) std::bind(&Application::x, this, std::placeholders::_1)

namespace MGE {

	Application* Application::s_Instance = nullptr;

	Application::Application() 
		: m_Camera(Vec4(-1.6f, 1.6f, -0.9f, 0.9f))
	{
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
		// glGenVertexArrays(1, &m_VertexArray);
		// glBindVertexArray(m_VertexArray); // The VAO is then bound using glBindVertexArray to set it as the active VAO.

		m_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f, /* x, y, z, and color�� depends on the layout*/
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		// upload the vertices to GPU to the currently bound VBO and specified as static draw data
		/* m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			BufferLayout layout = {
				{ ShaderDataType::Float4, "a_Color" }, // a element
				{ ShaderDataType::Float3, "a_Position" }
			};

			m_VertexBuffer->SetLayout(layout);
		}
*/
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(VertexArray::Create());
		// glEnableVertexAttribArray(0); // This enables the attribute at index 0 to be passed to the vertex shader during rendering.
		// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		/*
			so we made everything in a layout, it is easier to read and write
			in a layer there are many elements to tell shader how to interpreate the info in vertices
			
		*/

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };// represent the order in which the vertices should be connected to form triangles
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);
		

		/*
			The code generates an Index Buffer Object (IBO) using glGenBuffers. 
			An IBO is a buffer that stores the indices of the vertices that make up the primitive shapes.
		*/
		// glGenBuffers(2, &m_IndexBuffer);
		// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer); //  set it as the active buffer for storing index data.





		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_BlueShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
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
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();

			m_Camera.SetPosition({ 0.0f, 0.0f, 0.0f });
			m_Camera.SetRotation(mathter::Normalize(Quat(0.0f, 0.f, 0.f, 1.f)));

			Renderer::BeginScene(m_Camera);

			Renderer::Submit(m_SquareVA, m_BlueShader);
			Renderer::Submit(m_VertexArray, m_Shader);

			Renderer::EndScene();

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