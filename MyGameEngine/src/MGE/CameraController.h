#pragma once

#include "MGE/Renderer/Camera.h"
#include "MGE/Core/Timestep.h"

#include "MGE/Event/ApplicationEvent.h"
#include "MGE/Event/MouseEvent.h"


namespace MGE {

	class CameraController
	{
	public:
		CameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		Camera& GetCamera() { return m_Camera; }
		const Camera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 3.0f;
		Camera m_Camera;

		bool m_Rotation;

		Vec3 m_CameraPosition = { 0.0f, -2.5f, 0.0f };
		// Quat m_CameraRotation;
		float radian = 0.0f;
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
	};
}