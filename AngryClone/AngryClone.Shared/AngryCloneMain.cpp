#include "pch.h"
#include "AngryCloneMain.h"
#include "Common\DirectXHelper.h"

using namespace AngryClone;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

// Loads and initializes application assets when the application is loaded.
AngryCloneMain::AngryCloneMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
m_deviceResources(deviceResources), m_pointerLocationX(0.0f), m_level(std::unique_ptr<Level>(new Level()))
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	// TODO: Replace this with your app's content initialization.
//	m_sceneRenderer = std::unique_ptr<Sample3DSceneRenderer>(new Sample3DSceneRenderer(m_deviceResources));
	//my_sceneRenderer = std::unique_ptr<MyRenderer>(new MyRenderer(m_deviceResources));

//	m_fpsTextRenderer = std::unique_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(m_deviceResources));

	//!!!!!!!!!!!!!!!!!!!!!!!!!!m_renderer = new MyRenderer(m_deviceResources);
	SphereRenderer = new PseudoSphereRenderer(m_deviceResources);

	//m_renderer->Initialize(Windows::UI::Core::CoreWindow::GetForCurrentThread());
	//!!!!!!!!!!!!!!!!!!!!m_level->Initialise(m_renderer);
	m_level->Initialise(SphereRenderer);

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}

AngryCloneMain::~AngryCloneMain()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void AngryCloneMain::CreateWindowSizeDependentResources() 
{
	// TODO: Replace this with the size-dependent initialization of your app's content.
//	m_sceneRenderer->CreateWindowSizeDependentResources();
	//my_sceneRenderer->CreateWindowSizeDependentResources();

	//!!!!!!!!!!!!!!!!!!m_renderer->CreateWindowSizeDependentResources();
	SphereRenderer->CreateWindowSizeDependentResources();
}

void AngryCloneMain::StartRenderLoop()
{
	// If the animation render loop is already running then do not start another thread.
	if (m_renderLoopWorker != nullptr && m_renderLoopWorker->Status == AsyncStatus::Started)
	{
		return;
	}
	
	// Create a task that will be run on a background thread.
	auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction ^ action)
	{
		// Calculate the updated frame and render once per vertical blanking interval.
		while (action->Status == AsyncStatus::Started)
		{
			critical_section::scoped_lock lock(m_criticalSection);
			Update();
			if (Render())
			{
				m_deviceResources->Present();
			}
		}
	});

	// Run task on a dedicated high priority background thread.
	m_renderLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
}

void AngryCloneMain::StopRenderLoop()
{
	m_renderLoopWorker->Cancel();
}

// Updates the application state once per frame.
void AngryCloneMain::Update() 
{
	ProcessInput();

	// Update scene objects.
	m_timer.Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
//		m_sceneRenderer->Update(m_timer);
//		m_fpsTextRenderer->Update(m_timer);
		//my_sceneRenderer->Update(m_timer);
		
		m_level->Update();
		//!!!!!!!!!!!!!!!!!!!!!!!!m_renderer->Update(m_timer);
		SphereRenderer->Update(m_timer);
		//Windows::UI::Core::CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(Windows::UI::Core::CoreProcessEventsOption::ProcessAllIfPresent);
		//!!!!!!!!!!!!!!!!!!!!!!!!!!m_renderer->Render();
		SphereRenderer->Render();
		//m_renderer->Present(); // This call is synchronized to the display frame rate.
	});
}

// Process all input from the user before updating game state
void AngryCloneMain::ProcessInput()
{
	// TODO: Add per frame input handling here.
	//m_sceneRenderer->TrackingUpdate(m_pointerLocationX);
	//my_sceneRenderer->TrackingUpdate(m_pointerLocationX);
	
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool AngryCloneMain::Render() 
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.
//	m_sceneRenderer->Render();
//	m_fpsTextRenderer->Render();
	//my_sceneRenderer->Render();

	//!!!!!!!!!!!!!!!!!!!!!!!!m_renderer->Render();
	SphereRenderer->Render();

	return true;
}

// Notifies renderers that device resources need to be released.
void AngryCloneMain::OnDeviceLost()
{
	/*m_sceneRenderer->ReleaseDeviceDependentResources();
	m_fpsTextRenderer->ReleaseDeviceDependentResources();*/

	//!!!!!!!!!!!!!m_renderer->ReleaseDeviceDependentResources();
	SphereRenderer->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void AngryCloneMain::OnDeviceRestored()
{
	/*m_sceneRenderer->CreateDeviceDependentResources();
	m_fpsTextRenderer->CreateDeviceDependentResources();*/
	//my_sceneRenderer->CreateDeviceDependentResources();

	//!!!!!!!!!!!!!!!!!!m_renderer->CreateDeviceDependentResources();
	SphereRenderer->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();
}
