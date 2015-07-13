#pragma once
#include <btBulletDynamicsCommon.h>
#include "PseudoSphere.h"

class PseudoSphereMotionState : public btMotionState
{
public:
	PseudoSphereMotionState(PseudoSphere^ cube, const btTransform &initialTransform);
	virtual ~PseudoSphereMotionState(void);

	virtual void getWorldTransform(btTransform &worldTransform) const;
	virtual void setWorldTransform(const btTransform &worldTransform);
private:
	PseudoSphere^ Sphere;
	btTransform m_initialTransform;
};



