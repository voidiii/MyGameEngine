#pragma once
#include "MGE.h"

#include "imgui.h"
#include "MGE/ImGui/ImGuiLayer.h"
#include "MathAlias.h"
#include <MGE/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"
#include "Sandbox2D.h"

class ExampleLayer : public MGE::Layer 
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f, true)
	{
		/*
			The code generates a Vertex Array Object (VAO) using glGenVertexArrays. 
			A VAO is an object that stores the configurations of vertex attributes 
			(such as position, color, etc.) used for rendering. 
			It acts as a container for vertex buffer bindings and attribute configurations.
		*/
		// glGenVertexArrays(1, &m_VertexArray);
		// glBindVertexArray(m_VertexArray); // The VAO is then bound using glBindVertexArray to set it as the active VAO.

		m_VertexArray = MGE::VertexArray::Create();

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
		MGE::Ref<MGE::VertexBuffer> vertexBuffer = (MGE::VertexBuffer::Create(vertices, sizeof(vertices)));
		MGE::BufferLayout layout = {
			{ MGE::ShaderDataType::Float3, "a_Position" },
			{ MGE::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		uint32_t indices[3] = { 0, 1, 2 };
		MGE::Ref<MGE::IndexBuffer> indexBuffer = (MGE::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA = MGE::VertexArray::Create();
		// glEnableVertexAttribArray(0); // This enables the attribute at index 0 to be passed to the vertex shader during rendering.
		// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		/*a
			so we made everything in a layout, it is easier to read and write
			in a layer there are many elements to tell shader how to interpreate the info in vertices
			
		*/

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		MGE::Ref<MGE::VertexBuffer> squareVB = (MGE::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ MGE::ShaderDataType::Float3, "a_Position" },
			{ MGE::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };// represent the order in which the vertices should be connected to form triangles
		MGE::Ref<MGE::IndexBuffer> squareIB = MGE::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
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
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
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

		m_Shader = MGE::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_FlatColorShader = MGE::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

		// m_TextureShader.reset(MGE::Shader::Create("assets/shaders/Texture.glsl"));
		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = MGE::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = MGE::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<MGE::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<MGE::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
		
	}

	void OnUpdate(MGE::Timestep ts) override {

		m_CameraController.OnUpdate(ts);

		MGE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		MGE::RenderCommand::Clear();

		MGE::Renderer::BeginScene(m_CameraController.GetCamera());

		static MGE::Mat44 scale = mathter::Scale(MGE::Vec3(0.1f));
		//MGE::Renderer::Submit(m_SquareVA, m_BlueShader);

		std::dynamic_pointer_cast<MGE::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<MGE::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				MGE::Vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				MGE::Mat44 transform = (MGE::Mat44)mathter::Translation(pos) * scale;
				MGE::Renderer::Submit(m_SquareVA, m_FlatColorShader, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		// MGE::Renderer::Submit(m_VertexArray, m_Shader);
		m_Texture->Bind();
		MGE::Renderer::Submit(m_SquareVA, textureShader, mathter::Scale(MGE::Vec3(1.5f)));
		m_ChernoLogoTexture->Bind();
		MGE::Renderer::Submit(m_SquareVA, textureShader, mathter::Scale(MGE::Vec3(1.5f)));

		MGE::Renderer::EndScene();
	}

	void virtual OnImGuiRender(MGE::Timestep ts) override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", (float*)(&m_SquareColor));
		ImGui::End();
	}

	void OnEvent(MGE::Event& event) override {
		m_CameraController.OnEvent(event);
	}

private:
	MGE::ShaderLibrary m_ShaderLibrary;
	MGE::Ref<MGE::Shader> m_Shader;
	MGE::Ref<MGE::VertexArray> m_VertexArray;

	MGE::Ref<MGE::Shader> m_FlatColorShader;
	MGE::Ref<MGE::VertexArray> m_SquareVA;

	MGE::Ref<MGE::Texture2D> m_Texture, m_ChernoLogoTexture;

	MGE::CameraController m_CameraController;

	MGE::Vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };

};

class Sandbox : public MGE::Application {
public:
	Sandbox() {
		PushLayer(new Sandbox2D());
		
		// PushLayer(new ExampleLayer());
		//PushOverlay(new MGE::ImGuiLayer());
	}

	~Sandbox() {
	
	
	}

};

MGE::Application* MGE::CreateApplication() {
	return new Sandbox();
}