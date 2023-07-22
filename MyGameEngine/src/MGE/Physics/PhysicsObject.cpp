#include "MGEpch.h"
#include "PhysicsObject.h"

namespace MGE {

	TrianglePhyicsObject::TrianglePhyicsObject(Vec2 Position, Vec4 Color)
		: m_Position(Position), m_Color(Color)
	{
	}

	TrianglePhyicsObject::~TrianglePhyicsObject()
	{
	}

	void TrianglePhyicsObject::DrawPhysicsObject()
	{
		Renderer2D::DrawQuad(m_Position, { 0.1f, 0.2f }, m_Color);
	}

}