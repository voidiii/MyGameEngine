#include "MGEpch.h"
#include "Camera.h"

namespace MGE {

    Mat44 ortho
    (
        float const& left,
        float const& right,
        float const& bottom,
        float const& top,
        float const& zNear,
        float const& zFar
    )
    {
        Mat44 Result = mathter::Identity();;
        Result(0, 0) = 2.0f / (right - left);
        Result(1, 1) = 2.0f / (top - bottom);
        Result(2, 2) = -2.0f / (zFar - zNear);
        Result(3, 0) = -(right + left) / (right - left);
        Result(3, 1) = -(top + bottom) / (top - bottom);
        Result(3, 2) = -(zFar + zNear) / (zFar - zNear);
        return Result;
    }

    Camera::Camera(Vec4 left_right_bottom_top)
        : m_ProjectionMatrix(ortho(left_right_bottom_top[0],
                                   left_right_bottom_top[1], 
                                   left_right_bottom_top[2], 
                                   left_right_bottom_top[3], -1.0f, 1.0f)), m_ViewMatrix(mathter::Identity())
	{
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
        m_Position = Vec3(0.0f);
        m_Rotation = Quat(0.0f, 0.0f, 0.0f, -1.0f);
	}

	void Camera::RecalculateViewMatrix()
	{
        Mat44 IDMatrix = mathter::Identity();
        m_Rotation = Normalize(m_Rotation);
        Mat44 RotationMatrix = (Mat44)(m_Rotation);
        Mat44 transform = RotationMatrix * (IDMatrix + (Mat44)mathter::Translation(m_Position));

        m_ViewMatrix = mathter::Inverse(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}