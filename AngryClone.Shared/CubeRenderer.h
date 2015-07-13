#pragma once

#include <wrl.h>
#include <memory>

#include "Direct3DBase.h"
#include <DirectXMath.h>
#include "Content\ShaderStructures.h"

#include "Cube.h"

using namespace std;

//struct ModelViewProjectionConstantBuffer
//{
//	DirectX::XMFLOAT4X4 model;
//	DirectX::XMFLOAT4X4 view;
//	DirectX::XMFLOAT4X4 projection;
//};

ref class CubeRenderer : public Direct3DBase
{
public:
	virtual void Render() override;
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	void ReleaseDeviceDependentResources(); //!!!
	void Update();

//	void StartTracking(); //!!!!
//	void TrackingUpdate(float positionX); //!!!!!!
//	void StopTracking(); //!!!!!!!
//	bool IsTracking() { return m_tracking; } //!!!!!
private:
	~CubeRenderer();
	void InitialiseModelView();

	int singleRotator;
	void Rotate(float radians);
internal:
	CubeRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
	Cube^ CreateCube_old();
private:
	bool m_loadingComplete;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	uint32 m_indexCount;
	AngryClone::ModelViewProjectionConstantBuffer m_constantBufferData;
	vector<Cube^> m_cubes;

	// Variables used with the rendering loop.
//	bool	m_loadingComplete;
	float	m_degreesPerSecond;
//	bool	m_tracking;
};
