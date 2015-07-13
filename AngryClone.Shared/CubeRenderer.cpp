#include "pch.h"
#include "CubeRenderer.h"
#include <DirectXMath.h>
//#include "BasicLoader.h"

using namespace Microsoft::WRL;
using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace DirectX;

using namespace std;

CubeRenderer::CubeRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) : Direct3DBase(deviceResources),
m_loadingComplete(false),
m_indexCount(0), singleRotator(0)
{
}

CubeRenderer::~CubeRenderer()
{
}

void CubeRenderer::CreateDeviceResources()
{
	//Direct3DBase::CreateDeviceResources();

	//auto loadVSTask = DX::ReadDataAsync(L"SampleVertexShader.cso");
	//auto loadPSTask = DX::ReadDataAsync(L"SamplePixelShader.cso");

	//auto createVSTask = loadVSTask.then([this](const std::vector<byte>&  fileData) {
	//	DX::ThrowIfFailed(
	//		m_d3dDevice->CreateVertexShader(
	//		&fileData[0],
	//		fileData.size(),
	//		nullptr,
	//		&m_vertexShader
	//		)
	//		);

	//	const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	//	{
	//		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	};

	//	DX::ThrowIfFailed(
	//		m_d3dDevice->CreateInputLayout(
	//		vertexDesc,
	//		ARRAYSIZE(vertexDesc),
	//		&fileData[0],
	//		fileData.size(),
	//		&m_inputLayout
	//		)
	//		);
	//});

	//auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
	//	DX::ThrowIfFailed(
	//		m_d3dDevice->CreatePixelShader(
	//		&fileData[0],
	//		fileData.size(),
	//		nullptr,
	//		&m_pixelShader
	//		)
	//		);

	//	CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
	//	DX::ThrowIfFailed(
	//		m_d3dDevice->CreateBuffer(
	//		&constantBufferDesc,
	//		nullptr,
	//		&m_constantBuffer
	//		)
	//		);
	//});

	//auto createCubeTask = (createPSTask && createVSTask).then([this]() {
	//	VertexPositionColor cubeVertices[] =
	//	{
	//		{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
	//		{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
	//		{ XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
	//		{ XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f) },
	//		{ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
	//		{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f) },
	//		{ XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f) },
	//		{ XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f) },
	//	};

	//	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	//	vertexBufferData.pSysMem = cubeVertices;
	//	vertexBufferData.SysMemPitch = 0;
	//	vertexBufferData.SysMemSlicePitch = 0;
	//	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER);
	//	DX::ThrowIfFailed(
	//		m_d3dDevice->CreateBuffer(
	//		&vertexBufferDesc,
	//		&vertexBufferData,
	//		&m_vertexBuffer
	//		)
	//		);

	//	unsigned short cubeIndices[] =
	//	{
	//		0, 2, 1, // -x
	//		1, 2, 3,

	//		4, 5, 6, // +x
	//		5, 7, 6,

	//		0, 1, 5, // -y
	//		0, 5, 4,

	//		2, 6, 7, // +y
	//		2, 7, 3,

	//		0, 4, 6, // -z
	//		0, 6, 2,

	//		1, 3, 7, // +z
	//		1, 7, 5,
	//	};

	//	m_indexCount = ARRAYSIZE(cubeIndices);

	//	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	//	indexBufferData.pSysMem = cubeIndices;
	//	indexBufferData.SysMemPitch = 0;
	//	indexBufferData.SysMemSlicePitch = 0;
	//	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
	//	DX::ThrowIfFailed(
	//		m_d3dDevice->CreateBuffer(
	//		&indexBufferDesc,
	//		&indexBufferData,
	//		&m_indexBuffer
	//		)
	//		);
	//});

	//createCubeTask.then([this]() {
	//	m_loadingComplete = true;
	//	InitialiseModelView();
	//});

// Load shaders asynchronously.
auto loadVSTask = DX::ReadDataAsync(L"SampleVertexShader.cso");
auto loadPSTask = DX::ReadDataAsync(L"SamplePixelShader.cso");

// After the vertex shader file is loaded, create the shader and input layout.
auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateVertexShader(
		&fileData[0],
		fileData.size(),
		nullptr,
		&m_vertexShader
		)
		);

	static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateInputLayout(
		vertexDesc,
		ARRAYSIZE(vertexDesc),
		&fileData[0],
		fileData.size(),
		&m_inputLayout
		)
		);
});

// After the pixel shader file is loaded, create the shader and constant buffer.
auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreatePixelShader(
		&fileData[0],
		fileData.size(),
		nullptr,
		&m_pixelShader
		)
		);

	CD3D11_BUFFER_DESC constantBufferDesc(sizeof(AngryClone::ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
		&constantBufferDesc,
		nullptr,
		&m_constantBuffer
		)
		);
});

// Once both shaders are loaded, create the mesh.
auto createCubeTask = (createPSTask && createVSTask).then([this]() {

	// Load mesh vertices. Each vertex has a position and a color.
	static const VertexPositionColor cubeVertices[] =
	{
		{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
		{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f) },
		{ XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f) },
	};

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = cubeVertices;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
		&vertexBufferDesc,
		&vertexBufferData,
		&m_vertexBuffer
		)
		);

	// Load mesh indices. Each trio of indices represents
	// a triangle to be rendered on the screen.
	// For example: 0,2,1 means that the vertices with indexes
	// 0, 2 and 1 from the vertex buffer compose the 
	// first triangle of this mesh.
	static const unsigned short cubeIndices[] =
	{
		0, 2, 1, // -x
		1, 2, 3,

		4, 5, 6, // +x
		5, 7, 6,

		0, 1, 5, // -y
		0, 5, 4,

		2, 6, 7, // +y
		2, 7, 3,

		0, 4, 6, // -z
		0, 6, 2,

		1, 3, 7, // +z
		1, 7, 5,
	};

	m_indexCount = ARRAYSIZE(cubeIndices);

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = cubeIndices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
		&indexBufferDesc,
		&indexBufferData,
		&m_indexBuffer
		)
		);
});

// Once the cube is loaded, the object is ready to be rendered.
createCubeTask.then([this]() {
	m_loadingComplete = true;
});
}

void CubeRenderer::Update()
{
	if (singleRotator<m_cubes.size())
		Rotate(0);
}

void CubeRenderer::Rotate(float radians)
{
	// Prepare to pass the updated model matrix to the shader
	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(radians)));
	singleRotator++;
}

Cube^ CubeRenderer::CreateCube_old()
{
	auto cube = ref new Cube();
	m_cubes.push_back(cube);
	return cube;
}

void CubeRenderer::CreateWindowSizeDependentResources()
{
	//Direct3DBase::CreateWindowSizeDependentResources();

	//float aspectRatio = m_windowBounds.Width / m_windowBounds.Height;
	//float fovAngleY = 70.0f * XM_PI / 180.0f;

	//// Note that the m_orientationTransform3D matrix is post-multiplied here
	//// in order to correctly orient the scene to match the display orientation.
	//// This post-multiplication step is required for any draw calls that are
	//// made to the swap chain render target. For draw calls to other targets,
	//// this transform should not be applied.
	//XMStoreFloat4x4(
	//	&m_constantBufferData.projection,
	//	XMMatrixTranspose(
	//	XMMatrixMultiply(
	//	XMMatrixPerspectiveFovRH(
	//	fovAngleY,
	//	aspectRatio,
	//	0.01f,
	//	100.0f
	//	),
	//	XMLoadFloat4x4(&m_orientationTransform3D)
	//	)
	//	)
	//	);

	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// Note that the OrientationTransform3D matrix is post-multiplied here
	// in order to correctly orient the scene to match the display orientation.
	// This post-multiplication step is required for any draw calls that are
	// made to the swap chain render target. For draw calls to other targets,
	// this transform should not be applied.

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.01f,
		100.0f
		);

	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();

	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	XMStoreFloat4x4(
		&m_constantBufferData.projection,
		XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
		);

	// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
	static const XMVECTORF32 eye = { 7.0f, 0.7f, 1.5f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
}

void CubeRenderer::InitialiseModelView()
{
	XMVECTOR eye = XMVectorSet(-10.0f, 0.7f, -8.0f, 0.0f);
	XMVECTOR at = XMVectorSet(0.0f, 8.0f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
}

void CubeRenderer::Render()
{
	// clear
	/*m_d3dContext->OMSetRenderTargets(
		1,
		m_renderTargetView.GetAddressOf(),
		m_depthStencilView.Get()
		);

	const float midnightBlue[] = { 0.098f, 0.098f, 0.439f, 1.000f };
	m_d3dContext->ClearRenderTargetView(
		m_renderTargetView.Get(),
		midnightBlue
		);

	m_d3dContext->ClearDepthStencilView(
		m_depthStencilView.Get(),
		D3D11_CLEAR_DEPTH,
		1.0f,
		0
		);*/

	// only draw the cube once it's loaded (this is async)
	if (!m_loadingComplete)
		return;

	for (auto& cube : m_cubes)
	{
		XMStoreFloat4x4(&m_constantBufferData.model, cube->m_modelMatrix);

		m_d3dContext->UpdateSubresource(
			m_constantBuffer.Get(),
			0,
			NULL,
			&m_constantBufferData,
			0,
			0
			);

		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;
		m_d3dContext->IASetVertexBuffers(
			0,
			1,
			m_vertexBuffer.GetAddressOf(),
			&stride,
			&offset
			);

		m_d3dContext->IASetIndexBuffer(
			m_indexBuffer.Get(),
			DXGI_FORMAT_R16_UINT,
			0
			);

		m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_d3dContext->IASetInputLayout(m_inputLayout.Get());

		m_d3dContext->VSSetShader(
			m_vertexShader.Get(),
			nullptr,
			0
			);

		m_d3dContext->VSSetConstantBuffers(
			0,
			1,
			m_constantBuffer.GetAddressOf()
			);

		m_d3dContext->PSSetShader(
			m_pixelShader.Get(),
			nullptr,
			0
			);

		m_d3dContext->DrawIndexed(
			m_indexCount,
			0,
			0
			);
	}
}

void CubeRenderer::ReleaseDeviceDependentResources()
{
	m_loadingComplete = false;
	m_vertexShader.Reset();
	m_inputLayout.Reset();
	m_pixelShader.Reset();
	m_constantBuffer.Reset();
	m_vertexBuffer.Reset();
	m_indexBuffer.Reset();
}

//void CubeRenderer::StartTracking()
//{
//	m_tracking = true;
//}