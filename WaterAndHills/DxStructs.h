#ifndef _DXSTRUCTS_H_
#define _DXSTRUCTS_H_

#include "InclFiles.h"

class DxStructs
{
public:
	ID3D11RasterizerState *m_pNoCullRS;
	ID3D11RasterizerState *m_pBackCullRS; //Front is counter clockwise

	ID3D11SamplerState *m_pLinearWrapSampler;

	ID3D11BlendState *m_pTransparentBlender;
public:
	DxStructs();
	~DxStructs();

	BOOL InitStructs(ID3D11Device *pDevice);
};

#endif //_DXSTRUCTS_H_