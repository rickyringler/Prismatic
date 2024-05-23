#ifndef _DX_HPP_
#define _DX_HPP_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

class D3D
{

public:

	D3D();
	D3D(const D3D&);
	~D3D();

private:

	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_context = nullptr;
	D3D11_VIEWPORT* m_viewport = nullptr;
	ID3D11RenderTargetView* m_renderTarget = nullptr;
	IDXGISwapChain* m_swapChain = nullptr;
	ID3D11RasterizerState* m_rasterizerState = nullptr;
	D3D11_RASTERIZER_DESC* m_rasterizer = nullptr;
	ID3D11InputLayout* m_inputLayout = nullptr;
	D3D11_MAPPED_SUBRESOURCE* m_mapsr = nullptr;
	ID3D10Blob* m_psBlob = nullptr;
	ID3D10Blob* m_vsBlob = nullptr;
	ID3D11VertexShader* m_vs = nullptr;
	ID3D11PixelShader* m_ps = nullptr;
	ID3D11Buffer* m_vb = nullptr;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState m_stencilState = nullptr;
	ID3D10DepthStencilView m_stencilView = nullptr;
	DirectX::XMMATRIX m_projectionMatrix = nullptr;
	DirectX::XMMATRIX m_worldMatrix = nullptr;
	DirectX::XMMATRIX m_orthographicMatrix = nullptr;
	
	bool m_vsync = false;
	int m_VRAM;
	char m_video_card[128];


	bool Init(HWND hwnd,int,int,bool,bool,float,float);
	void Shutdown(void);
	void GetProjectionM(DirectX::XMMATRIX&);
	void GetWorldM(DirectX::XMMATRIX&);
	void GetOrthographicM(DirectX::XMMATRIX&);
	void GetGPU(char&,int&);
	void InitRenderTarget(void);
	void ResetViewport(void);
	void RenderScene(float,float,float,float);
	void EndScene(void);

};

#endif