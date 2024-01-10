#pragma once
#include "../PhysicsObject.h"
#include "MGE/Core/ProfilingTimer.h"

#include <unordered_map>
#include <utility>


class FluidScene
{
public:
	FluidScene(float height, float width, int numberOfObjects);
	~FluidScene() {
	}

private:
	int m_NumberOfObjects;

};