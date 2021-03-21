#ifndef _DXMANAGER_H_
#define _DXMANAGER_H_

#include "DxClass.h"
#include "ShaderClass.h"
#include "TextureClass.h"
#include "DxStructs.h"
#include "VertexTypes.h"
#include "GenerateObjects.h"
#include "CameraClass.h"
#include "InputClass.h"

//field sizes
#define X_WIDTH 10.f
#define Z_WIDTH 10.f

struct MatrixData //Constant buffer for matrices
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
};

struct WaterData //Constant buffer for water shader linked with "misc" cbuffer
{
	XMFLOAT4X4 WaterTexMove;
};

class DxManager : public DxClass
{
protected:
	ID3D11Buffer *m_pVertexBuffer;
	ID3D11Buffer *m_pIndexBuffer;
	ID3D11Buffer *m_pMatrixBuffer;
	ID3D11Buffer *m_pWaterConstBuffer;
	ID3D11Buffer *m_pAlphaConstBuffer;

	ShaderClass *m_pBasicShader;
	ShaderClass *m_pWaterShader;

	TextureClass m_Textures;
	CameraClass m_Camera;
	

	DxStructs *m_pDxStructs;
	std::vector<UINT> m_IndexCount;
	std::vector<UINT> m_VertexCount;
	float m_WaterTrans;
public:
	InputClass *m_Input;
public:
	DxManager();
	~DxManager();

	BOOL InitInstance(HWND hwnd);

	void Update();
	void Render();
protected:
	BOOL InitBuffers();
	void UpdateMatrixBuffer(CXMMATRIX World, CXMMATRIX View, CXMMATRIX Projection);
	void UpdateWaterConstBuffer(CXMMATRIX WaterTexMove);

	BOOL LoadTextures(string texFile);
};

#endif //_DXMANAGER_H_