#include "Sandbox2D.h"
#include "imgui.h"
#include <thread>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f, true), /*m_PhysicsScene(50.0f, 100.0f, 500)*/  m_CollisionDetectionScene(10.0f, 100.0f, 2)
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
	{
		m_CameraController.OnUpdate(ts);
	}

	// Render
	{
		MGE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		MGE::RenderCommand::Clear();
	}
	{
		MGE::Renderer2D::BeginScene(m_CameraController.GetCamera());

		// m_PhysicsScene.OnUpdate(ts);
		m_CollisionDetectionScene.OnUpdate(ts);
		
		MGE::Renderer2D::EndScene();
	}

}

void Sandbox2D::OnImGuiRender(MGE::Timestep ts)
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", (float*)(&m_SquareColor));
	static float minFrameRate = 1.0f / ts.GetFrameRate();
	minFrameRate = minFrameRate < 1.0f / ts.GetFrameRate() ? minFrameRate : 1.0f / ts.GetFrameRate();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", ts.GetFrameRate() * 1000.0f, (1.0f / ts.GetFrameRate()) > 120.0f ? 120.0f : (1.0f / ts.GetFrameRate()));

	// ImGui::Text("The number of balls: %d", m_PhysicsScene.GetNumberOfObjects());

	// for (auto& result : m_PhysicsScene.GetProfileResult())
	// {
	// 	char label[50];
	// 	strcpy(label, "%.3fms ");
	// 	strcat(label, result.Name);
	// 	ImGui::Text(label, result.Time);
	// }
	// m_PhysicsScene.GetProfileResult().clear();

	ImGui::End();
}

void Sandbox2D::OnEvent(MGE::Event& e)
{
	m_CameraController.OnEvent(e);
}