#include "DxStructs.h"

////////////////////////////////////////////////////////////
//		Constructors And Destructors
//

DxStructs::DxStructs() //set all pointers to null, so we can be able to check if they were used
{
	m_pNoCullRS = nullptr;
	m_pBackCullRS = nullptr;
	m_pLinearWrapSampler = nullptr;
	m_pTransparentBlender = nullptr;
}

DxStructs::~DxStructs() //check if object (i.e. pointer) is used, and if it is perform safe releasing
{
	//note that due to "Safe Release" declaration, object will always be set to NULL, but release will be called only if it exists
	if (m_pNoCullRS)
		SafeRelease(m_pNoCullRS);
	if (m_pBackCullRS)
		SafeRelease(m_pBackCullRS);
	if (m_pLinearWrapSampler)
		SafeRelease(m_pLinearWrapSampler);
	if (m_pTransparentBlender)
		SafeRelease(m_pTransparentBlender);
}

////////////////////////////////////////////////////////////
//		Protected Functions
//

BOOL DxStructs::InitStructs(ID3D11Device *pDevice)
{
	HRESULT hr;

	D3D11_RASTERIZER_DESC rd;

	ZeroMemory(&rd, sizeof(rd));
	rd.CullMode = D3D11_CULL_NONE;
	rd.FillMode = D3D11_FILL_SOLID;
	rd.DepthClipEnable = TRUE;
	rd.FrontCounterClockwise = TRUE;
	rd.MultisampleEnable = TRUE;
	rd.DepthBiasClamp = 0.f;
	rd.SlopeScaledDepthBias = 0.f;
	rd.DepthBias = FALSE;
	rd.ScissorEnable = FALSE;
	rd.AntialiasedLineEnable = FALSE;

	hr = pDevice->CreateRasterizerState(&rd, &m_pNoCullRS);
	CheckHR(hr);

	rd.CullMode = D3D11_CULL_BACK;

	hr = pDevice->CreateRasterizerState(&rd, &m_pBackCullRS);
	CheckHR(hr);

	D3D11_SAMPLER_DESC sd;

	ZeroMemory(&sd, sizeof(sd));
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MinLOD = 0.f;
	sd.MaxAnisotropy = 1;

	hr = pDevice->CreateSamplerState(&sd, &m_pLinearWrapSampler);
	CheckHR(hr);

	D3D11_BLEND_DESC bd;

	ZeroMemory(&bd, sizeof(bd));
	bd.AlphaToCoverageEnable = FALSE;
	bd.IndependentBlendEnable = FALSE;

	bd.RenderTarget[0].BlendEnable = TRUE;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = pDevice->CreateBlendState(&bd, &m_pTransparentBlender);
	CheckHR(hr);

	return 0;
}