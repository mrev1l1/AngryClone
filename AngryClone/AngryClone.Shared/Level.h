#pragma once

#include "pch.h"
#include "Cube.h"
#include "Physics.h"
#include "CubeRenderer.h"
#include "MyRenderer.h"

class Level
{
public:
	Level(void);
	virtual ~Level(void);

	void Initialise(MyRenderer* cubeRenderer);
	void Update();
private:
	Physics m_physics;
	std::vector<Cube^> m_cubes;
};

