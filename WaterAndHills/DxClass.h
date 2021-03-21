#ifndef _DXCLASS_H_
#define _DXCLASS_H_

#include "InclFiles.h"

class DxClass
{
	//*** vars ***
public:
	UINT m_ScrWidth, m_ScrHeight;
protected:
	HWND m_hwnd;
	

	ID3D11Device *m_pDevice;
	ID3D11DeviceContext *m_pDeviceContext;
	ID3D11RenderTargetView *m_pRenderTargetView;
	ID3D11DepthStencilView *m_pDepthStencilView;
	IDXGISwapChain *m_pSwapChain;

	ID3D11Texture2D *m_pDepthBuffer;

	XMMATRIX m_world;
	XMMATRIX m_projection;

	//*** functions ***
public:
	DxClass();
	~DxClass();

	virtual void Render() = 0;
protected:
	BOOL InitDeviceAndSwapChain(UINT MSAA_Count, UINT *MSAA_Quality, D3D_FEATURE_LEVEL *FeatureLevel, BOOL windowed);
	BOOL InitRenderTargets(UINT MSAA_Count, UINT MSAA_Quality);
	void InitViewPort();
};

#endif