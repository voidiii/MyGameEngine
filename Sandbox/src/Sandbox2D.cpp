#include "Sandbox2D.h"
#include "imgui.h"
#include <thread>
#include <filesystem>
#include <iomanip>
#include <ctime>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), 
		m_CameraController(1280.0f / 720.0f, true), 
		m_PhysicsScene(20.0f, 20.0f, 200),
		m_BallScene(80.0f, 130.0f, 10000, false),
		m_CollisionDetectionScene(10.0f, 100.0f, 2),
		m_Scene_SpringAndSoftbody(10.0f, 100.0f, 2)
{
}

void Sandbox2D::OnAttach()
{
	m_PerformanceStart = std::chrono::steady_clock::now();
	std::error_code logError;
	std::filesystem::create_directories("logs", logError);
	const auto stamp = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()).count();
	m_PerformanceLogPath = "logs/performance-" + std::to_string(stamp) + ".csv";
	if (!logError) m_PerformanceLog.open(m_PerformanceLogPath);
	if (m_PerformanceLog)
	{
		m_PerformanceLog << "build_id,config,elapsed_s,scene,balls,frame_ms,fps,physics_step_ms,physics_steps_per_second,spawn_rate,initial_speed\n";
		m_PerformanceLog.flush();
	}
	m_BallCamera.SetZoomLevel(92.0f);
	// This camera has rotation controls disabled; explicitly start upright.
	m_BallCamera.GetCamera().SetRotation(0.0f);
	m_CheckerboardTexture = MGE::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{

	
}

void Sandbox2D::OnUpdate(MGE::Timestep ts)
{
	// Update
	{
		if (m_ActiveScene == Scene::BallCollision) m_BallCamera.OnUpdate(ts);
		else m_CameraController.OnUpdate(ts);
		if (m_ActiveScene == Scene::BallCollision)
		{
			auto& io = ImGui::GetIO();
			if (!io.WantCaptureMouse && MGE::Input::IsMouseButtonPressed(0))
			{
				auto& window = MGE::Application::Get().GetWindow();
				const auto mouse = MGE::Input::GetMousePosition();
				const float width = static_cast<float>(window.GetWidth());
				const float height = static_cast<float>(window.GetHeight());
				if (width > 0 && height > 0 && mouse.first >= 0 && mouse.first < width && mouse.second >= 0 && mouse.second < height)
				{
					const auto& camera = m_BallCamera.GetCamera();
					const float zoom = m_BallCamera.GetZoomLevel();
					MGE::Vec2 position{camera.GetPosition().x + (2.0f * mouse.first / width - 1.0f) * zoom * width / height,
						camera.GetPosition().y + (1.0f - 2.0f * mouse.second / height) * zoom};
					m_SpawnAccumulator += std::min(static_cast<float>(ts), 0.05f) * m_SpawnRate;
					while (m_SpawnAccumulator >= 1.0f) { m_BallScene.SpawnBall(position, m_SpawnSpeed); m_SpawnAccumulator -= 1.0f; }
				}
			}
			else m_SpawnAccumulator = 0.0f;
		}
	}

	// Render
	{
		MGE::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		MGE::RenderCommand::Clear();
	}
	{
		MGE::Renderer2D::BeginScene(m_ActiveScene == Scene::BallCollision ? m_BallCamera.GetCamera() : m_CameraController.GetCamera());

		switch (m_ActiveScene)
		{
		case Scene::Fluid: m_PhysicsScene.OnUpdate(ts); break;
		case Scene::BallCollision: m_BallScene.OnUpdate(ts); break;
		case Scene::Collision: m_CollisionDetectionScene.OnUpdate(ts); break;
		case Scene::SpringAndSoftbody: m_Scene_SpringAndSoftbody.OnUpdate(ts); break;
		}
		
		MGE::Renderer2D::EndScene();
	}

}

void Sandbox2D::OnImGuiRender(MGE::Timestep ts)
{
	const int logScene = static_cast<int>(m_ActiveScene);
	if (logScene != m_LogScene)
	{
		m_LogScene = logScene;
		m_LogInterval = m_LogFrameSeconds = 0.0;
		m_LogFrames = 0;
	}
	m_LogInterval += std::max(0.0f, static_cast<float>(ts));
	m_LogFrameSeconds += std::max(0.0f, static_cast<float>(ts));
	++m_LogFrames;
	if (m_LogInterval >= 1.0 && m_PerformanceLog)
	{
		const char* names[] = {"fluid", "polygon_collision", "spring_softbody", "ball_collision"};
		const bool particles = m_ActiveScene == Scene::Fluid || m_ActiveScene == Scene::BallCollision;
		const auto& scene = m_ActiveScene == Scene::BallCollision ? m_BallScene : m_PhysicsScene;
		const double elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - m_PerformanceStart).count();
		m_PerformanceLog << std::fixed << std::setprecision(6)
			<< PERF_BUILD_ID << ',' << PERF_BUILD_CONFIG << ',' << elapsed << ',' << names[logScene] << ','
			<< (particles ? scene.GetNumberOfObjects() : 0) << ','
			<< m_LogFrameSeconds * 1000.0 / m_LogFrames << ',' << m_LogFrames / m_LogFrameSeconds << ',';
		if (particles) m_PerformanceLog << scene.GetPhysicsStepMilliseconds() << ',' << scene.GetPhysicsStepsPerSecond();
		else m_PerformanceLog << ',';
		m_PerformanceLog << ',' << m_SpawnRate << ',' << m_SpawnSpeed << '\n';
		m_PerformanceLog.flush();
		m_LogInterval = m_LogFrameSeconds = 0.0;
		m_LogFrames = 0;
	}
	ImGui::Begin("Settings");
	ImGui::TextWrapped(m_PerformanceLog ? "Performance CSV logging: ON (logs folder)" : "Performance CSV logging: FAILED");
	int selectedScene = static_cast<int>(m_ActiveScene);
	if (ImGui::Combo("Scene", &selectedScene,
		"Particle Fluid\0Polygon Collision\0Spring & Soft Body\0Ball Collision\0"))
		m_ActiveScene = static_cast<Scene>(selectedScene);
	ImGui::TextWrapped("Inactive scenes are paused. Switching back resumes their state.");
	const float frameTime = ts.GetFrameRate();
	ImGui::Text("Application %.3f ms/frame (%.1f FPS)", frameTime * 1000.0f,
		frameTime > 0.0f ? 1.0f / frameTime : 0.0f);
	ImGui::Separator();

	if (m_ActiveScene == Scene::Fluid)
	{
	ImGui::Text("The number of balls: %d", m_PhysicsScene.GetNumberOfObjects());
	float PreesureMuti = m_PhysicsScene.GetPressureMultipier();
	float TargetDensity = m_PhysicsScene.GetTargetDensity();
	ImGui::DragFloat("Pressure multiplier", &PreesureMuti, 0.05f);
	ImGui::DragFloat("Target density", &TargetDensity, 0.05f);
	m_PhysicsScene.SetPressureMultipier(PreesureMuti);
	m_PhysicsScene.SetTargetDensity(TargetDensity);
	
	for (auto& result : m_PhysicsScene.GetProfileResult())
	{
		ImGui::Text("%.3fms %s", result.Time, result.Name);
	}
	}
	else if (m_ActiveScene == Scene::BallCollision)
	{
		ImGui::Text("Balls: %d", m_BallScene.GetNumberOfObjects());
		ImGui::Text("Spawn limit: 50,000 balls");
		ImGui::Text("Physics step (avg): %.3f ms", m_BallScene.GetPhysicsStepMilliseconds());
		ImGui::Text("Physics steps / second: %.1f", m_BallScene.GetPhysicsStepsPerSecond());
		ImGui::TextDisabled("Updated every ~1 s. Target: 480 steps/s.");
		ImGui::TextWrapped("Hold left mouse in the scene to add balls. Gravity: 9.81. Bounds: 260 x 160. Single-threaded grid collisions, fixed 480 Hz physics.");
		ImGui::SliderFloat("Balls / second", &m_SpawnRate, 10.0f, 1000.0f, "%.0f");
		ImGui::SliderFloat("Initial speed", &m_SpawnSpeed, 1.0f, 30.0f, "%.1f");
		for (const auto& result : m_BallScene.GetProfileResult())
			ImGui::Text("%.3fms %s", result.Time, result.Name);
	}
	else
	{
		ImGui::TextWrapped(m_ActiveScene == Scene::Collision
			? "Polygon collision experiment. No scene parameters exposed yet."
			: "Spring and soft body experiment. No scene parameters exposed yet.");
	}

	ImGui::End();
}

void Sandbox2D::OnEvent(MGE::Event& e)
{
	if (m_ActiveScene == Scene::BallCollision) m_BallCamera.OnEvent(e);
	else m_CameraController.OnEvent(e);
}
