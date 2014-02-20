/*
================================
 d3dmanager.h

 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "d3dbase.h"


/*
================
 D3DManager::D3DManager
================
*/
D3DBase::D3DBase() :
	m_swapChain                (nullptr),
	m_device                   (nullptr),
	m_deviceContext            (nullptr),
	m_renderTargetView         (nullptr),
	m_depthStencilBuffer       (nullptr),
	m_depthStencilState        (nullptr),
	m_depthStencilView         (nullptr),
	m_rasterizerState          (nullptr),
	m_disableDepthStencilState (nullptr)
{

}

D3DBase::D3DBase(const D3DBase& other)
{

}

/*
================
 D3DManager::~D3DManager
================
*/
D3DBase::~D3DBase(void)
{
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}
}

/*
================
 D3DManager::init

 Sets up device, swap chain, back buffer, depth buffer and matrices.
================
*/
// FIXME: split into multiple methods for readability
bool D3DBase::init( int screenWidth, int screenHeight, const bool vsync, HWND hwnd, const bool fullscreen, const float screenDepth, const float screenNear )
{
	HRESULT result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes;	
	unsigned int numerator;
	unsigned int denominator;
	unsigned int stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;	
	float fov;
	float aspectRatio;
	D3D11_RASTERIZER_DESC rasterizerDesc;

	// Set the vsync setting
	m_vsyncEnabled = vsync;

	// Get refresh rates and find a correct one to use
	// ----------------------------------------------------------------------
		
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		return false;
	}

	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	displayModeList = new DXGI_MODE_DESC[numModes];
	if(!displayModeList)
	{
		return false;
	}
	
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if (FAILED(result))
	{
		return false;
	}

	// Find correct display mode
	for (unsigned int i = 0; i < numModes; ++i)
	{
		if (displayModeList[i].Width == (unsigned int) screenWidth)
		{
			if (displayModeList[i].Height == (unsigned int) screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// Get GPU name and memory
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		return false;
	}

	// Store memory in megabytes
	m_gpuMemory = (int) (adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Get GPU name
	char gpuDescription[128];
	error = wcstombs_s(&stringLength, gpuDescription, 128, adapterDesc.Description, 128);
	if (error != 0)
	{
		return false;
	}

	m_gpuDescription = std::string(gpuDescription);

	// Clean up
	delete[] displayModeList;
	displayModeList = NULL;

	adapterOutput->Release();
	adapterOutput = NULL;

	adapter->Release();
	adapter = NULL;

	factory->Release();
	factory = 0;

	//------------------------------------------------------------------------------
	
	// Start actual directx initialization
	// ------------------------------------------------------------------------------------

	// Swap chain
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (m_vsyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;

	// no multisampling = 1, 0
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	

	if (fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	// Set the feature level (change to 10 or 9 for backwards compatibility)
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Finally create the device (note it's directX version specific)
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);
	if (FAILED(result))
	{
		return false;		
	}

	// Back buffer
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*) &backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, m_renderTargetView.GetAddressOf());
	if (FAILED(result))
	{
		return false;
	}

	// Clean up
	backBufferPtr->Release();
	backBufferPtr = NULL;

	//  -- Back buffer

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;  // 1
	depthBufferDesc.SampleDesc.Quality = 0;  // 0
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// -- Depth Stencil

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 1);

	// Initialize depth stencil view
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create depth stencil view
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), &depthStencilViewDesc, &m_depthStencilView);

	// Bind render target view
	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	// -- Rasterizer state

	// Setup the raster description which will determine how and what polygons will be drawn.
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;  // D3D11_FILL_WIREFRAME
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;  // false
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	result = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState);
	if(FAILED(result))
	{
		return false;
	}

	m_deviceContext->RSSetState(m_rasterizerState.Get());

	// Setup the viewport
	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(screenWidth);
	viewport.Height = static_cast<float>(screenHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;

	// Create the viewport.
	m_deviceContext->RSSetViewports(1, &viewport);

	// Setup projection matrix
	fov = (float) D3DX_PI / 4.0f;
	aspectRatio = (float) screenWidth / (float) screenHeight;

	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fov, aspectRatio, screenNear, screenDepth);

	// Initialize world matrix
	D3DXMatrixIdentity(&m_worldMatrix);

	// Create camera here

	// Create an orthographic projection matrix for 2D rendering
	D3DXMatrixOrthoLH(&m_orthoMatrix, (float) screenWidth, (float) screenHeight, screenNear, screenDepth);

	// Depth stencil stuff
	// ----------------------------------------------------------------------------------------------
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Create second depth stencil state which turns off the Z buffer for 2D rendering
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device
	result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_disableDepthStencilState);
	if (FAILED(result))
	{
		return false;
	}
	
	return true;
}


/*
================
 D3DManager::beginScene

 Clears render target and back buffer to prepare for rendering.
================
*/
void D3DBase::beginScene( float red, float green, float blue, float alpha )
{
	float color[4];
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	// Clear the back buffer
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), color);

	// Clear the depth buffer
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

/*
================
 D3DManager::endScene

 Shows the rendered image.
================
*/
void D3DBase::endScene()
{
	// Present the back buffer to the screen when the rendering is complete
	if (m_vsyncEnabled)
	{
		m_swapChain->Present(1,0);
	}
	else
	{
		m_swapChain->Present(0,0);  // present asap
	}
}

/*
================
 D3DManager::getDevice
================
*/
ID3D11Device* D3DBase::getDevice() const
{
	return m_device.Get();
}

ID3D11DeviceContext* D3DBase::getDeviceContext() const
{
	return m_deviceContext.Get();
}

/*
================
 D3DManager::getProjectionMatrix
================
*/
void D3DBase::getProjectionMatrix( D3DXMATRIX& projectionMatrix ) const
{
	projectionMatrix = m_projectionMatrix;
}

/*
================
 D3DManager::getWorldMatrix
================
*/
void D3DBase::getWorldMatrix( D3DXMATRIX& worldMatrix ) const
{
	worldMatrix = m_worldMatrix;
}

/*
================
 D3DManager::getOrthoMatrix
================
*/
void D3DBase::getOrthoMatrix( D3DXMATRIX& orthoMatrix ) const
{
	orthoMatrix = m_orthoMatrix;
}

/*
================
 D3DManager::getGpuInfo
================
*/
void D3DBase::getGpuInfo( std::string& gpuName, int& memory ) const
{
	gpuName = m_gpuDescription;
	memory = m_gpuMemory;
}

/*
================
 D3DManager::turnZBufferOn
================
*/
void D3DBase::turnZBufferOn()
{
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), 1);
}

/*
================
 D3DManager::turnZBufferOff
================
*/
void D3DBase::turnZBufferOff()
{
	m_deviceContext->OMSetDepthStencilState(m_disableDepthStencilState.Get(), 1);
}

/*
================
 D3DManager::getDepthStencilView
================
*/
ID3D11DepthStencilView* D3DBase::getDepthStencilView() const
{
	return m_depthStencilView.Get();
}

/*
================
 D3DManager::setBackBufferRenderTarget
================
*/
void D3DBase::setBackBufferRenderTarget()
{
	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
}

void D3DBase::resetViewport(float width, float height)
{
	if (m_swapChain)
	{
		m_deviceContext->OMSetRenderTargets(0, 0, 0);        
        m_renderTargetView.ReleaseAndGetAddressOf();
		
		HRESULT hr = m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

		// Add error handling

		// Get buffer and create a render target view
		ID3D11Texture2D* pBuffer;
		hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**) &pBuffer);

		hr = m_device->CreateRenderTargetView(pBuffer, NULL, m_renderTargetView.ReleaseAndGetAddressOf());

		pBuffer->Release();

        m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), NULL);
        // setBackBufferRenderTarget();
		
		// Setup projection matrix
		float fov = (float) D3DX_PI / 4.0f;
		float aspectRatio = width / height;
		D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fov, aspectRatio, 1.0f, 1000.0f);
		
		// Setup the viewport
		D3D11_VIEWPORT viewport;
		viewport.Width = width;
		viewport.Height = height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;

		m_deviceContext->RSSetViewports(1, &viewport);
	}	
}
