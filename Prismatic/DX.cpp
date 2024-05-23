#include "DX.hpp"

D3D::D3D(){}
D3D::D3D(const D3D&){}
D3D::~D3D(){}

bool D3D::Init(HWND hwnd,int width,int height,bool vsync,bool fullscreen,float depth,float near)
{
	HRESULT* result;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* output;
	DXGI_MODE_DESC* modeListDesc;
	DXGI_ADAPTER_DESC* adapterDesc;
	DXGI_SWAP_CHAIN_DESC* swapChainDesc;
	D3D_FEATURE_LEVEL* featureLevel;
	ID3D11Texture2D* backBuffer;
	D3D11_TEXTURE2D_DESC* depthBufferDesc;
	ID3D11DepthStencilState* depthStencilState;
	D3D11_DEPTH_STENCIL_DESC* depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC* depthStencilViewDesc;
	D3D11_RASTERIZER_DESC* rasterizerDesc;

	unsigned int modes;
	unsigned int it;
	unsigned int numerator;
	unsigned int denominator;
	unsigned long long string_len;
	int error_num;
	float field_of_view;
	float screen_aspect;

	m_vsync = vsync;

	result = CreateDXGIFactory(__uuidof(IDXGIFactory), void(**)& factory);
	if (FAILED(result)){return false;}
	
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result) return false;);

	result = adapter->EnumOutputs(0, &output);
	if (FAILED(result) return false;);

	result = output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modes, NULL);
	if (FAILED(result) return false;);

	modeListDesc = new DXGI_MODE_DESC[modes];
	if (FAILED(result) return false;);

	for(i=0;i<modes;i++)
	{
		if(modeListDesc[i].Width == (unsigned int)width)
		{
			if(modeListDesc[i].Height == (unsigned int)height)
			{
				numerator = modeListDesc[i].RefreshRate.Numerator;
				denominator = modeListDesc[i].RefreshRate.Denominator
			}
		}
	}

	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result) return false;);

	m_VRAM = (int)(adapterDesc->DedicatedVideoMemory / 1024 / 1024);

	error_num = wcstombs_s(&string_len, m_video_card, 128, adapterDesc->Description, 128);
	if (error_num != 0) { return false };

	delete[]modeListDesc;

	output->Release();
	adapter->Release();
	factory->Release();
	
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc->BufferCount = 1;
	swapChainDesc->BufferDesc.Width = width;
	swapChainDesc->BufferDesc.Height = height;
	swapChainDesc->BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

	if(m_vsync)
	{
		swapChainDesc->BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc->BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc->BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc->BufferDesc.RefreshRate.Denominator = 1;
	}
	swapChainDesc->BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc->SampleDesc.Count = 1;
	swapChainDesc->SampleDesc.Quality = 0;
	swapChainDesc->BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc->BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc->SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc->Flags = 0;
	if(fullscreen){swapChainDesc->Windowed=false;}else{swapChainDesc->Windowed=true;}

	featureLevel = D3D_FEATURE_LEVEL_11_0;
	
	result = D3D11CreateDeviceAndSwapChain
	(
		NULL,
		D3D10_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_swapChain,
		&m_device,
		NULL,
		m_context
	);
	if (FAILED(result) return false;);

	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(result) return false;);

	result = m_device->CreateRenderTargetView(backBuffer, NULL, &m_renderTarget);
	if (FAILED(result) return false;);
	
	backBuffer->Release();

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc->Width = width;
	depthBufferDesc->Height = height;
	depthBufferDesc->MipLevels = 1;
	depthBufferDesc->ArraySize = 1;
	depthBufferDesc->Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc->SampleDesc.Count = 1;
	depthBufferDesc->SampleDesc.Quality = 0;
	depthBufferDesc->Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc->BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc->CPUAccessFlags = 0;
	depthBufferDesc->MiscFlags = 0;

	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilDesc);
	if (FAILED(result) return false;);

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc->DepthEnable = true;
	depthStencilDesc->DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc->DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc->StencilEnable = true;
	depthStencilDesc->StencilReadMask = 0xFF;
	depthStencilDesc->StencilWriteMask = 0xFF;
	depthStencilDesc->FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc->FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc->FrontFace.StencilPassOp = D3D11_STENCIL_OR_KEEP;
	depthStencilDesc->FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc->BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc->BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc->BackFace.StencilPassOp = D3D11_STENCIL_OR_KEEP;
	depthStencilDesc->BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_stencilState);
	if (FAILED(result) return false;);
	m_context->OMGetDepthStencilState(depthStencilState, 1);
	
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc);

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MlpSlice = 0;

	result = m_device->CreateDepthStencilView(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if (FAILED(result) return false;);

	m_context->OMSetRenderTargets(1, &m_renderTarget, m_stencilView);

	rasterizerDesc->AntialiasedLineEnable = false;
	rasterizerDesc->CullMode = D3D11_CULL_BACK;
	rasterizerDesc->DepthBias = 0;
	rasterizerDesc->DepthBiasClamp = 0.0f;
	rasterizerDesc->FillMode = D3D11_FILL_SOLID;
	rasterizerDesc->FrontCounterClockwise = false;
	rasterizerDesc->MultisampleEnable = false;
	rasterizerDesc->ScissorEnable = false;
	rasterizerDesc->SlopeScaledDepthBias = 0.0f;

	result = m_device->CreateRasterizerState(&rasterizerDesc, m_rasterizerState);
	if (FAILED(result) return false;);

	m_context->RSSetState(m_rasterizerState);

	m_viewport->Width = (float)width;
	m_viewport->Height = (float)height;
	m_viewport->MinDepth = 0.0f;
	m_viewport->MaxDepth = 1.0f;
	m_viewport->TopLeftX = 0.0f;
	m_viewport->TopLeftY = 0.0f;

	m_context->RSSetViewports(1, &m_viewport);

	field_of_view = 3.141592654f / 4.0f;
	screen_aspect = (float)width / (float)height;

	m_projectionMatrix = DirectX::XMMATRIXPerspectiveFovLH(field_of_view, screen_aspect, screen_near, screen_depth);
	m_worldMatrix = DirectX::XMMatrixIdentity();
	m_orthographicMatrix = DirectX::XMMatrixOrthographicLH((float)width, (float)height, screen_near, screen_depth);

	return true;

}
void D3D::Shutdown(void){}
void D3D::GetProjectionM(DirectX::XMMATRIX&){}
void D3D::GetWorldM(DirectX::XMMATRIX&){}
void D3D::GetOrthographicM(DirectX::XMMATRIX&){}
void D3D::GetGPU(char&,int&){}
void D3D::InitRenderTarget(void){}
void D3D::ResetViewport(void){}
void D3D::RenderScene(float,float,float,float){}
void D3D::EndScene(void){}