#pragma once
#include <wrl.h>
#include <memory>
#include "Cube.h"
#include <DirectXMath.h>

ref class PseudoSphere
{
private:
	~PseudoSphere();
internal:
	PseudoSphere();
	void SetTransform(const Transform^ transform);

	DirectX::XMMATRIX m_modelMatrix;
private:
};

