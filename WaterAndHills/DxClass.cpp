#include "DxClass.h"

////////////////////////////////////////////////////////////
//		Constructors And Destructors
//

DxClass::DxClass() //set all pointers to null, so we can be able to check if they were used
{
	m_pDevice = nullptr;
	m_pDeviceContext = nullptr;
	m_pSwapChain = nullptr;
	m_pDepthStencilView = nullptr;
	m_pDepthBuffer = nullptr;
	m_pRenderTargetView = nullptr;
}

DxClass::~DxClass() //check if object (i.e. pointer) is used, and if it is perform safe releasing
{
	if (m_pSwapChain) //note that due to "Safe Release" declaration, object will always be set to nullptr, but release will be called only if it exists
		SafeRelease(m_pSwapChain);
	if (m_pDepthBuffer)
		SafeRelease(m_pDepthBuffer);
	if (m_pDepthStencilView)
		SafeRelease(m_pDepthStencilView);
	if (m_pRenderTargetView)
		SafeRelease(m_pRenderTargetView);
	if (m_pDeviceContext)
		SafeRelease(m_pDeviceContext);
	if (m_pDevice)
		SafeRelease(m_pDevice);
}

////////////////////////////////////////////////////////////
//		Protected Functions
//

BOOL DxClass::InitDeviceAndSwapChain(UINT MSAA_Count, UINT *MSAA_Quality, D3D_FEATURE_LEVEL *FeatureLevel, BOOL windowed)
{
	IDXGIFactory *pFactory;
	IDXGIAdapter *pAdapter;
	IDXGIDevice *pDevice;

	HRESULT hr;
	UINT flags = 0;
	RECT rc;

	GetWindowRect(m_hwnd, &rc);
	m_ScrWidth = rc.right - rc.left;
	m_ScrHeight = rc.bottom - rc.top;

#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif 


	hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, nullptr, 0, D3D11_SDK_VERSION, &m_pDevice, FeatureLevel, &m_pDeviceContext);
	CheckHR(hr);


	/*Note possible leaks if one of hoops fails*/
	hr = m_pDevice->QueryInterface<IDXGIDevice>(&pDevice);
	CheckHR(hr);

	hr = pDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter);
	CheckHR(hr);

	hr = pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);
	CheckHR(hr);
	/*End of possible leaks*/

	//check MSAA levels
	hr = m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, MSAA_Count, MSAA_Quality);
	CheckHR(hr);

	/*Create swap chain*/
	DXGI_SWAP_CHAIN_DESC sd;

	ZeroMemory(&sd, sizeof(sd));

#ifdef _DEBUG
	sd.BufferCount = 1; //enable debug device to track memory
#else
	sd.BufferCount = 2;
#endif //_DEBUG
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.Width = m_ScrWidth;
	sd.BufferDesc.Height = m_ScrHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.Flags = 0;
	sd.OutputWindow = m_hwnd;
	sd.SampleDesc.Count = MSAA_Count;
	sd.SampleDesc.Quality = *MSAA_Quality - 1;
	sd.Windowed = windowed;
	
	hr = pFactory->CreateSwapChain(m_pDevice, &sd, &m_pSwapChain);
	CheckHR(hr);
	/*End of SwapChain creation*/
	
	SafeRelease(pFactory);
	SafeRelease(pAdapter);
	SafeRelease(pDevice);

	return 0;
}

BOOL DxClass::InitRenderTargets(UINT MSAA_Count, UINT MSAA_Quality)
{
	ID3D11Texture2D *pBackBuffer;
	HRESULT hr;

	/*RendewrTargetView Creation*/
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	CheckHR(hr);

	hr = m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
	CheckHR(hr);
	/*End of render target view creation*/

	/*Depth stencil view and depth buffer creation*/
	D3D11_TEXTURE2D_DESC td;

	ZeroMemory(&td, sizeof(td));
	td.ArraySize = 1;
	td.MipLevels = 1;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.Height = m_ScrHeight;
	td.Width = m_ScrWidth;
	td.SampleDesc.Count = MSAA_Count;
	td.SampleDesc.Quality = MSAA_Quality - 1;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.MiscFlags = 0;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.CPUAccessFlags = 0;

	hr = m_pDevice->CreateTexture2D(&td, nullptr, &m_pDepthBuffer);
	CheckHR(hr);

	hr = m_pDevice->CreateDepthStencilView(m_pDepthBuffer, nullptr, &m_pDepthStencilView);
	CheckHR(hr);
	/*End of deptencil view and buffer creation*/

	//Set Depth stenil and render target view to use
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	return 0;
}


void DxClass::InitViewPort()
{
	D3D11_VIEWPORT vp;

	ZeroMemory(&vp, sizeof(vp));
	vp.Width = (float)m_ScrWidth;
	vp.Height = (float)m_ScrHeight;
	vp.MaxDepth = 1.f;
	vp.MinDepth = 0.f;
	vp.TopLeftX = 0.f;
	vp.TopLeftY = 0.f;

	m_pDeviceContext->RSSetViewports(1, &vp);
}