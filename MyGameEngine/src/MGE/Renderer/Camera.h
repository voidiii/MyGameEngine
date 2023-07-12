#pragma once

#include "MathAlias.h"

namespace MGE {

	class Camera
	{
	public:
        Camera(Vec4 left_right_bottom_top);

        void SetPosition(const Vec3& position) { m_Position = position; RecalculateViewMatrix(); }
        void SetRotation(float radian) { 

            m_Rotation.w = std::cos(radian / 2);
            //m_Rotation.x = m_Rotation.x * std::sin(radian / 2);
            //m_Rotation.y = m_Rotation.y * std::sin(radian / 2);
            //m_Rotation.z = m_Rotation.z * std::sin(radian / 2);
            RecalculateViewMatrix(); 
        }
        
        const Vec3& GetPosition() const { return m_Position; }
        Quat GetRotation() const { return m_Rotation; }

        const Mat44& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const Mat44& GetViewMatrix() const { return m_ViewMatrix; }
        const Mat44& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

    private:
        void RecalculateViewMatrix();
    private:
        Mat44 m_ProjectionMatrix;
        Mat44 m_ViewMatrix;
        Mat44 m_ViewProjectionMatrix;

        Vec3 m_Position;
        Quat m_Rotation;
        // float m_radian = 0.0f;

	};

}

