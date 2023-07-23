#include "Sandbox2D.h"
#include "imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true), m_PhysicsScene()
{
}

void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = MGE::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{

	
}

void Sandbox2D::OnUpdate(MGE::Timestep ts)
{
	// Update
	// PROFILE_SCOPE("Sandbox2D::OnUpdate");
	MGE_PROFILE_FUNCTION();

	{
		//PROFILE_SCOPE("m_CameraController::OnUpdate");
		MGE_PROFILE_FUNCTION();
		m_CameraController.OnUpdate(ts);
	}

	// Render
	{
		// PROFILE_SCOPE("Renderer Prep");
		MGE_PROFILE_FUNCTION();
		MGE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		MGE::RenderCommand::Clear();
	}
	{
		// PROFILE_SCOPE("Renderer Draw");
		MGE_PROFILE_FUNCTION();
		MGE::Renderer2D::BeginScene(m_CameraController.GetCamera());

		m_PhysicsScene.OnUpdate(ts);

		for(int i = 0 ; i < 10; i++)
		{
			m_PhysicsScene.GetPhysicsObjects(i)->OnUpdate(ts);
		}
		
		for (int i = 0; i < 10; i++)
		{
			m_PhysicsScene.GetPhysicsObjects(i)->DrawPhysicsObject();
		}
		// MGE::Renderer2D::DrawCircle(MGE::Vec3{0.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 0.8f, 1.0f });
		MGE::Renderer2D::EndScene();
	}

}

void Sandbox2D::OnImGuiRender()
{
	MGE_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", (float*)(&m_SquareColor));

	ImGui::End();
}

void Sandbox2D::OnEvent(MGE::Event& e)
{
	m_CameraController.OnEvent(e);
}