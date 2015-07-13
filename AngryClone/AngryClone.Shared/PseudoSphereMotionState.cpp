#include "pch.h"
#include "PseudoSphereMotionState.h"



PseudoSphereMotionState::PseudoSphereMotionState(PseudoSphere^ sphere, const btTransform &initialTransform)
	:
	Sphere(sphere),
	m_initialTransform(initialTransform)
{
	this->setWorldTransform(m_initialTransform);
}

PseudoSphereMotionState::~PseudoSphereMotionState(void)
{
}

void PseudoSphereMotionState::getWorldTransform(btTransform &worldTransform) const
{
	worldTransform = m_initialTransform;
}

void PseudoSphereMotionState::setWorldTransform(const btTransform &worldTransform)
{
	auto rotation = worldTransform.getRotation();

	auto transform = ref new Transform(worldTransform.getOrigin().getX(),
		worldTransform.getOrigin().getY(),
		worldTransform.getOrigin().getZ(),
		rotation.getX(), rotation.getY(), rotation.getZ(), rotation.getW());

	Sphere->SetTransform(transform);
}
