#include "ShaderClass.h"

ShaderClass::ShaderClass()
{
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	m_pGeometryShader = nullptr;
	m_pInputLayout = nullptr;
}

ShaderClass::~ShaderClass()
{
	if (m_pVertexShader)
		SafeRelease(m_pVertexShader);
	if (m_pPixelShader)
		SafeRelease(m_pPixelShader);
	if (m_pGeometryShader)
		SafeRelease(m_pGeometryShader);
	if (m_pInputLayout)
		SafeRelease(m_pInputLayout);
}

BOOL ShaderClass::InitShaderCode(ID3D11Device *m_pDevice, D3D11_INPUT_ELEMENT_DESC *ied, int numElement, LPWSTR lpVertexName, LPWSTR lpPixelName, LPWSTR lpGeometryName)
{
	ID3DBlob *pCompiledVS, *pCompiledPS, *pCompiledGS = nullptr, *pErr = nullptr;
	HRESULT hr;


	hr = D3DCompileFromFile(lpVertexName, nullptr, nullptr, "main", "vs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pCompiledVS, &pErr);
	if (FAILED(hr))
	{
		if (pErr == nullptr)
		{
			MessageBox(nullptr, L"Missing Vertex Shader File", L"Error", MB_OK | MB_ICONWARNING);
		}
		else
		{
			MessageBoxA(nullptr, (LPSTR)pErr->GetBufferPointer(), "Error", MB_OK | MB_ICONWARNING);
		}

		return 1;
	}

	if (lpGeometryName)
	{
		hr = D3DCompileFromFile(lpGeometryName, nullptr, nullptr, "main", "gs_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pCompiledGS, &pErr);
		if (FAILED(hr))
		{
			if (pErr == nullptr)
			{
				MessageBox(nullptr, L"Missing Geometry Shader File", L"Error", MB_OK | MB_ICONWARNING);
			}
			else
			{
				MessageBoxA(nullptr, (LPSTR)pErr->GetBufferPointer(), "Error", MB_OK | MB_ICONWARNING);
			}

			return 1;
		}
	}

	hr = D3DCompileFromFile(lpPixelName, nullptr, nullptr, "main", "ps_5_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, &pCompiledPS, &pErr);
	if (FAILED(hr))
	{
		if (pErr == nullptr)
		{
			MessageBox(nullptr, L"Missing Pixel Shader File", L"Error", MB_OK | MB_ICONWARNING);
		}
		else
		{
			MessageBoxA(nullptr, (LPSTR)pErr->GetBufferPointer(), "Error", MB_OK | MB_ICONWARNING);
		}

		return 1;
	}


	hr = m_pDevice->CreateVertexShader(pCompiledVS->GetBufferPointer(), pCompiledVS->GetBufferSize(), nullptr, &m_pVertexShader);
	CheckHR(hr);

	if (lpGeometryName)
	{
		hr = m_pDevice->CreateGeometryShader(pCompiledGS->GetBufferPointer(), pCompiledGS->GetBufferSize(), nullptr, &m_pGeometryShader);
		CheckHR(hr);
	}

	hr = m_pDevice->CreatePixelShader(pCompiledPS->GetBufferPointer(), pCompiledPS->GetBufferSize(), nullptr, &m_pPixelShader);
	CheckHR(hr);

	hr = m_pDevice->CreateInputLayout(ied, numElement, pCompiledVS->GetBufferPointer(), pCompiledVS->GetBufferSize(), &m_pInputLayout);
	CheckHR(hr);

	return 0;
}
