#include "pch.h"
#include "PseudoSphereRenderer.h"

#include "Common\DirectXHelper.h"

using namespace AngryClone;

using namespace DirectX;
using namespace Windows::Foundation;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
PseudoSphereRenderer::PseudoSphereRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
m_loadingComplete(false),
m_degreesPerSecond(45),
m_indexCount(0),
m_tracking(false),
m_deviceResources(deviceResources)
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
	singleRotator = 0;
}

// Initializes view parameters when the window size changes.
void PseudoSphereRenderer::CreateWindowSizeDependentResources()
{
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
	static const XMVECTORF32 eye = { -10.0f, 0.7f, -8.0f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, 8.0f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	/*static const XMVECTORF32 eye = { 0.f, 0.7f, 3.625f, 0.0f };
	static const XMVECTORF32 at = { -0.f, -0.1f, -0.f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };*/

	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
}

// Called once per frame, rotates the cube and calculates the model and view matrices.
void PseudoSphereRenderer::Update(DX::StepTimer const& timer)
{
	if (!m_tracking)
	{
		// Convert degrees to radians, then convert seconds to rotation angle
		float radiansPerSecond = XMConvertToRadians(m_degreesPerSecond);
		double totalRotation = timer.GetTotalSeconds() * radiansPerSecond;
		float radians = static_cast<float>(fmod(totalRotation, XM_2PI));

		//if (singleRotator<1)
		Rotate(radians);
	}
}

// Rotate the 3D cube model a set amount of radians.
void PseudoSphereRenderer::Rotate(float radians)
{
	// Prepare to pass the updated model matrix to the shader
	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(radians)));
	//Render();
	singleRotator++;
}


// Renders one frame using the vertex and pixel shaders.
void PseudoSphereRenderer::Render()
{
	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!m_loadingComplete)
	{
		return;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();
	
	XMStoreFloat4x4(&m_constantBufferData.model, Sphere->m_modelMatrix);

	// Prepare the constant buffer to send it to the graphics device.
	context->UpdateSubresource(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0
		);

	// Each vertex is one instance of the VertexPositionColor struct.
	UINT stride = sizeof(AngryClone::VertexPositionColor);
	UINT offset = 0;
	context->IASetVertexBuffers(
		0,
		1,
		m_vertexBuffer.GetAddressOf(),
		&stride,
		&offset
		);

	context->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
		0
		);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->IASetInputLayout(m_inputLayout.Get());

	// Attach our vertex shader.
	context->VSSetShader(
		m_vertexShader.Get(),
		nullptr,
		0
		);

	// Send the constant buffer to the graphics device.
	context->VSSetConstantBuffers(
		0,
		1,
		m_constantBuffer.GetAddressOf()
		);

	// Attach our pixel shader.
	context->PSSetShader(
		m_pixelShader.Get(),
		nullptr,
		0
		);

	// Draw the objects.
	context->DrawIndexed(
		m_indexCount,
		0,
		0
		);
}

void PseudoSphereRenderer::CreateDeviceDependentResources()
{
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

		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
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
		static const AngryClone::VertexPositionColor cubeVertices[] =
		{
			/*{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f) },
			{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
			{ XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
			{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f) },
			{ XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f) },
			{ XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f) },*/

			{ XMFLOAT3(-0.5f, 0.5f, -0.25f), XMFLOAT3(1.0f, .0f, .0f) }, //0
			{ XMFLOAT3(-0.25f, 0.5f, -0.5f), XMFLOAT3(.0f, .0f, 1.0f) }, //1
			{ XMFLOAT3(-0.5f, 0.25f, -0.5f), XMFLOAT3(1.0f, .0f, .0f) }, //2
			
			{ XMFLOAT3(0.25f, 0.5f, 0.5f), XMFLOAT3(0.875f, .0f, .0f) },
			{ XMFLOAT3(0.5f, 0.25f, -0.5f), XMFLOAT3(.0f, 1.0f, .0f) }, //4
			{ XMFLOAT3(0.5f, 0.5f, -0.25f), XMFLOAT3(.0f, 1.0f, 0.f) }, //5
			
			{ XMFLOAT3(0.5f, -0.25f, -0.5f), XMFLOAT3(0.f, 1.0f, .0f) }, //6
			{ XMFLOAT3(0.25f, -0.5f, -0.5f), XMFLOAT3(.0f, 0.0f, 1.0f) }, //7
			{ XMFLOAT3(0.5f, -0.5f, -0.25f), XMFLOAT3(.0f, 1.0f, 0.f) }, //8
			
			{ XMFLOAT3(0.5f, -0.5f, 0.25f), XMFLOAT3(0.f, 1.0f, .0f) }, //9
			{ XMFLOAT3(0.5f, -0.25f, 0.5f), XMFLOAT3(.0f, 1.f, .0f) }, //10
			{ XMFLOAT3(0.25f, -0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f) }, //11

			{ XMFLOAT3(-0.25f, -0.5f, 0.5f), XMFLOAT3(0.1f, 1.0f, .0f) }, //12
			{ XMFLOAT3(-0.5f, -0.25f, 0.5f), XMFLOAT3(1.0f, 0.f, .0f) }, //13
			{ XMFLOAT3(-0.5f, -0.5f, 0.25f), XMFLOAT3(1.0f, .0f, 0.f) }, //14

			{ XMFLOAT3(-0.5f, 0.25f, 0.5f), XMFLOAT3(1.f, .0f, .0f) }, //15
			{ XMFLOAT3(-0.25f, 0.5f, 0.5f), XMFLOAT3(1.0f, 1.f, .0f) }, //16
			{ XMFLOAT3(-0.5f, 0.5f, 0.25f), XMFLOAT3(1.0f, .0f, 0.0f) }, //17

			{ XMFLOAT3(-0.5f, -0.5f, -0.25f), XMFLOAT3(1.0f, .0f, .0f) }, //18
			{ XMFLOAT3(-0.5f, -0.25f, -0.5f), XMFLOAT3(.0f, 0.f, 1.0f) }, //19
			{ XMFLOAT3(-0.25f, -0.5f, -0.5f), XMFLOAT3(.0f, .0f, 1.0f) }, //20

			{ XMFLOAT3(0.25f, 0.5f, 0.5f), XMFLOAT3(1.f, 1.0f, .0f) }, //21
			{ XMFLOAT3(0.5f, 0.5f, 0.25f), XMFLOAT3(.0f, 1.0f, .0f) }, //22
			{ XMFLOAT3(0.5f, 0.25f, 0.5f), XMFLOAT3(.0f, 1.0f, 0.0f) }, //23
			{ XMFLOAT3(0.25f, 0.5f, -0.5f), XMFLOAT3(.0f, .0f, 1.0f) }, //24
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
			//0, 2, 1, // -x
			//1, 2, 3,

			//4, 5, 6, // +x
			//5, 7, 6,

			//0, 1, 5, // -y
			//0, 5, 4,

			//2, 6, 7, // +y
			//2, 7, 3,

			//0, 4, 6, // -z
			//0, 6, 2,

			//1, 3, 7, // +z
			//1, 7, 5,

			//!!!!
			13,14,15,
			14,17,15,
			14,18,17,
			0,17,18,
			0,18,19,
			19,2,0,

			//!!!!!
			4,6,5,
			6,8,5,
			8,22,5,
			8,9,22,
			23,22,9,
			9,10,23,
			
			//!!!!!
			9, 8, 7,
			9, 7, 11,
			11, 7, 20,
			11, 20, 12,
			12, 20, 18,
			12, 18, 14,
			//12,14,18,

			//!!!!!
			1,24,5,
			1,5,22,
			16,17,22,
			16,22,21,
			22, 0, 1,
			0,22,17,
			
			//22, 1, 21,
			//22,21,1,
			//21, 1, 3,
			//21, 3, 22,
			//22, 3, 5,

			//6, 4, 3, //- hernya
			//6, 3, 7, //- hernya
			//!!!!!!!!!!!!!!!
			1,7,4,
			7,6,4,
			1,4,24,
			7, 1, 20,
			20, 1, 2,
			20, 2, 19,

			//!!!!!!!!!!!!
			12, 13, 15, 
			12, 15, 16,
			12, 16, 11,
			11, 16, 21,
			11, 21, 10,
			10, 21, 23,

			1, 0, 2,
			15,17,16,
			17, 22, 20,
			13, 12, 14,
			18, 20, 19,
			4, 3, 5,
			//22, 21, 23,
			21,22,23,
			6, 7, 8,
			//6,8,7,
			//10, 11, 9
			10,9,11,
			24,4,5
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

void PseudoSphereRenderer::ReleaseDeviceDependentResources()
{
	m_loadingComplete = false;
	m_vertexShader.Reset();
	m_inputLayout.Reset();
	m_pixelShader.Reset();
	m_constantBuffer.Reset();
	m_vertexBuffer.Reset();
	m_indexBuffer.Reset();
}

PseudoSphere^ PseudoSphereRenderer::CreatePseudoSphere()
{
	Sphere = ref new PseudoSphere();
	return Sphere;
}

void PseudoSphereRenderer::StartTracking()
{
	m_tracking = true;
}

// When tracking, the 3D cube can be rotated around its Y axis by tracking pointer position relative to the output screen width.
void PseudoSphereRenderer::TrackingUpdate(float positionX)
{
	if (m_tracking)
	{
		float radians = XM_2PI * 2.0f * positionX / m_deviceResources->GetOutputSize().Width;
		Rotate(radians);
	}
}

void PseudoSphereRenderer::StopTracking()
{
	m_tracking = false;
}