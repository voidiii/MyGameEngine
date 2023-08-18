#include "MGEpch.h"
#include "StaticObject.h"

namespace MGE {

	void StaticObject_Line::ApplySupport(Timestep ts, Ref<MassPoint> mass_point)
	{
		mass_point->ChangeVelocity(-2.0f * mathter::Dot(m_Normal, mass_point->GetVelocity()) * m_Normal + mass_point->GetVelocity());
	}

}