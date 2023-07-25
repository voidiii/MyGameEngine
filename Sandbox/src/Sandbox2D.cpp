#include "Sandbox2D.h"
#include "imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true), m_PhysicsScene(5.0f, 30.0f, 500)
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
		
		for (int i = 0; i < m_PhysicsScene.GetNumberOfObjects(); i++)
		{
			m_PhysicsScene.GetPhysicsObjects(i)->DrawPhysicsObject();
		}
		MGE::Renderer2D::EndScene();
	}

}

void Sandbox2D::OnImGuiRender(MGE::Timestep ts)
{
	MGE_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", (float*)(&m_SquareColor));
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1.0f / ts.GetFrameRate(), (1.0f / ts.GetFrameRate()) > 120.0f ? 120.0f : (1.0f / ts.GetFrameRate()));

	ImGui::Text("Sample object's speed: %.3f , %.3f", m_PhysicsScene.GetPhysicsObjects(0)->GetVelocity().x, m_PhysicsScene.GetPhysicsObjects(0)->GetVelocity().y);

	ImGui::End();
}

void Sandbox2D::OnEvent(MGE::Event& e)
{
	m_CameraController.OnEvent(e);
}