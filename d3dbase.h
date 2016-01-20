/*
================================
 D3DBase.h

 Created on: 09 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#pragma warning( disable : 4005 )


#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DX11.lib")
#pragma comment(lib, "d3dx10.lib")


#include <string>
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <D3DX10math.h>

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

/*
================================
 
 D3DBase

 A wrapper class for Direct3D initialization.
 Creates the device, device context, sets up swap chain and the back buffer.
 
================================
*/
class D3DBase
{
public:
    D3DBase(void);
    D3DBase(const D3DBase&);
    ~D3DBase(void);

	bool init( int screenWidth, int screenHeight, const bool vsync, HWND hwnd, const bool fullscreen, const float screenDepth, const float screenNear );
	
	void beginScene( float r, float g, float b, float a );
	void endScene();

	ID3D11Device* getDevice() const;
	ID3D11DeviceContext* getDeviceContext() const;

	void getProjectionMatrix(D3DXMATRIX&) const;
	void getWorldMatrix(D3DXMATRIX&) const;
	void getOrthoMatrix(D3DXMATRIX&) const;
	
	void getGpuInfo(std::string& description, int& memory) const;	
	
	ID3D11DepthStencilView* getDepthStencilView() const;

	void turnZBufferOn();
	void turnZBufferOff();
	
	void resetViewport(float width, float height);

private:
	bool m_vsyncEnabled;
	int m_gpuMemory;
	std::string m_gpuDescription;
	ComPtr<IDXGISwapChain> m_swapChain;
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;
	ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
	ComPtr<ID3D11DepthStencilState> m_depthStencilState;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	ComPtr<ID3D11RasterizerState> m_rasterizerState;
	D3DXMATRIX m_projectionMatrix;
	D3DXMATRIX m_worldMatrix;
	D3DXMATRIX m_orthoMatrix;
	ComPtr<ID3D11DepthStencilState> m_disableDepthStencilState;	

	void setBackBufferRenderTarget();
};

