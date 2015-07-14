#pragma once

#include "pch.h"
#include "Cube.h"
#include "Physics.h"
#include "CubeRenderer.h"
#include "MyRenderer.h"
#include "PseudoSphere.h"
#include "PseudoSphereRenderer.h"
class Level
{
public:
	Level(void);
	virtual ~Level(void);

	void Initialise(MyRenderer* cubeRenderer);
	void Initialise(PseudoSphereRenderer* sphereRenderer);
	void Update();
	void Kick();
	void Punch();
private:
	Physics m_physics;
	std::vector<Cube^> m_cubes;
	PseudoSphere^ Sphere;
};

