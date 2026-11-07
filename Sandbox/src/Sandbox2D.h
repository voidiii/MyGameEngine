#pragma once

#include "MGE.h"
#include "MathAlias.h"
#include "MGE/Core/ProfilingTimer.h"
#include "MGE/Physics/Scene_Ball.h"
#include "MGE/Physics/Scene_CollisionDetection.h"
#include "MGE/Physics/Scene_SpringAndSoftbody.h"
#include <fstream>
#include <chrono>


class Sandbox2D : public MGE::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(MGE::Timestep ts) override;
	virtual void OnImGuiRender(MGE::Timestep ts) override;
	void OnEvent(MGE::Event& e) override;
private:
	enum class Scene { Fluid, Collision, SpringAndSoftbody, BallCollision };
	Scene m_ActiveScene = Scene::Fluid;

	MGE::CameraController m_CameraController;
	MGE::CameraController m_BallCamera{1280.0f / 720.0f, false};
	float m_SpawnAccumulator = 0.0f;
	float m_SpawnRate = 200.0f;
	float m_SpawnSpeed = 15.0f;
	std::ofstream m_PerformanceLog;
	std::string m_PerformanceLogPath;
	std::chrono::steady_clock::time_point m_PerformanceStart;
	double m_LogInterval = 0.0;
	double m_LogFrameSeconds = 0.0;
	int m_LogFrames = 0;
	int m_LogScene = -1;

	// Temp
	MGE::Ref<MGE::VertexArray> m_SquareVA;
	MGE::Ref<MGE::Shader> m_FlatColorShader;
	MGE::Ref<MGE::Texture2D> m_CheckerboardTexture;

	MGE::PhysicsScene m_PhysicsScene;
	MGE::PhysicsScene m_BallScene;
	MGE::CollisionDetectionScene m_CollisionDetectionScene;
	MGE::Scene_SpringAndSoftbody m_Scene_SpringAndSoftbody;

	std::vector<MGE::ProfileResult> m_ProfileResults;

	MGE::Vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};
