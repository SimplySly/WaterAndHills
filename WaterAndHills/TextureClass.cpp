#include "TextureClass.h"

////////////////////////////////////////////////////////////
//		Constructors And Destructors
//

TextureClass::~TextureClass()
{
	for (UINT i = 0; i < m_pTextureSRV.size(); i++)
	{
		m_pTextureSRV[i]->Release();
	}
}


////////////////////////////////////////////////////////////
//		Protected Functions
//

ID3D11ShaderResourceView** TextureClass::operator[] (wstring name) 
{
	return &m_pTextureSRV[m_names[name]];
}



BOOL TextureClass::AddTextureDDS(ID3D11Device *pDevice, LPCWSTR name, LPCWSTR alternateName)
{
	HRESULT hr;

	m_pTextureSRV.resize(m_pTextureSRV.size() + 1); //add space for one more

	if (alternateName)//add name to array of names
	{
		m_names.insert(pair<wstring, int>(alternateName, m_pTextureSRV.size() - 1));
	}
	else
	{
		m_names.insert(pair<wstring, int>(name, m_pTextureSRV.size() - 1));
	}

	m_pTextureSRV[m_pTextureSRV.size() - 1] = nullptr;
	hr = CreateDDSTextureFromFile(pDevice, name, nullptr, &m_pTextureSRV[m_pTextureSRV.size() - 1]); //Load texture
	if (FAILED(hr))
	{
		m_pTextureSRV.resize(m_pTextureSRV.size() - 1); //if it fails remove added extra space
		if (alternateName)//erase from the list if failed
		{
			m_names.erase(alternateName);
		}
		else
		{
			m_names.erase(name);
		}
		return 1;
	}

	return 0; //everything is okay
}

BOOL TextureClass::AddTextureWIC(ID3D11Device *pDevice, LPCWSTR name, LPCWSTR alternateName)
{
	HRESULT hr;

	m_pTextureSRV.resize(m_pTextureSRV.size() + 1); //add space for one more

	if (alternateName)//add name to array of names
	{
		m_names.insert(pair<wstring, int>(alternateName, m_pTextureSRV.size() - 1));
	}
	else
	{
		m_names.insert(pair<wstring, int>(name, m_pTextureSRV.size() - 1));
	}

	m_pTextureSRV[m_pTextureSRV.size() - 1] = nullptr;
	hr = CreateWICTextureFromFile(pDevice, name, nullptr, &m_pTextureSRV[m_pTextureSRV.size() - 1]); //Load texture
	if (FAILED(hr))
	{
		m_pTextureSRV.resize(m_pTextureSRV.size() - 1); //if it fails remove added extra space
		if (alternateName)//erase from the list if failed
		{
			m_names.erase(alternateName);
		}
		else
		{
			m_names.erase(name);
		}
		return 1;
	}

	return 0;
}