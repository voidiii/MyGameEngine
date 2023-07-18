#include "MGEpch.h"
#include "CameraController.h"

#include "MGE/Input.h"
#include "MGE/KeyCodes.h"

namespace MGE {

    CameraController::CameraController(float aspectRatio, bool rotation)
        : m_AspectRatio(aspectRatio), 
            m_Camera(Vec4(- m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)),
            m_Rotation(rotation)
    {
    }
    
    void CameraController::OnUpdate(Timestep ts)
    {
        if (Input::IsKeyPressed(MGE_KEY_A))
            m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
        else if (Input::IsKeyPressed(MGE_KEY_D))
            m_CameraPosition.x += m_CameraTranslationSpeed * ts;

        if (Input::IsKeyPressed(MGE_KEY_W))
            m_CameraPosition.y += m_CameraTranslationSpeed * ts;
        else if (Input::IsKeyPressed(MGE_KEY_S))
            m_CameraPosition.y -= m_CameraTranslationSpeed * ts;

        if (m_Rotation)
        {
            if (Input::IsKeyPressed(MGE_KEY_Q))
                radian += m_CameraRotationSpeed * ts * 0.01f;
            if (Input::IsKeyPressed(MGE_KEY_E))
                radian -= m_CameraRotationSpeed * ts * 0.01f;

            m_Camera.SetRotation(radian);
        }

        m_Camera.SetPosition(m_CameraPosition);

        m_CameraTranslationSpeed = m_ZoomLevel;
    }
    
    void CameraController::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(MGE_BIND_EVENT_FUNCTION(CameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(MGE_BIND_EVENT_FUNCTION(CameraController::OnWindowResized));
    }
    
    bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
    {
        m_ZoomLevel -= e.GetYOffset() * 0.25f;
        m_ZoomLevel = max(m_ZoomLevel, 0.25f);
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }
    
    bool CameraController::OnWindowResized(WindowResizeEvent& e)
    {
        m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }
    
}


