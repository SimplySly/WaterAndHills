#ifndef _SHADERCLASS_H_
#define _SHADERCLASS_H_

#include "InclFiles.h"

class ShaderClass
{
public:
	ID3D11VertexShader *m_pVertexShader;
	ID3D11PixelShader *m_pPixelShader;
	ID3D11GeometryShader *m_pGeometryShader;
	
	ID3D11InputLayout *m_pInputLayout;
public:
	ShaderClass();
	~ShaderClass();

	BOOL InitShaderCode(ID3D11Device *m_pDevice, D3D11_INPUT_ELEMENT_DESC *ied, int numElement, LPWSTR lpVertexName, LPWSTR lpPixelName, LPWSTR lpGeometryName = NULL);
};

#endif //_SHADERCLASS_H_