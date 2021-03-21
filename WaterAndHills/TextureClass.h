#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

#include "InclFiles.h"

/*NOTE: to use this file DirectXTK must be properly linked to solution*/

class TextureClass
{
protected:
	vector<ID3D11ShaderResourceView*> m_pTextureSRV;
	unordered_map<wstring, int> m_names;
public:
	ID3D11ShaderResourceView** operator [] (wstring name); //it's enough to know name of texture and it's ready to use
	inline ID3D11ShaderResourceView** operator [] (int index) //faster processing, but requires you to know index of resource
	{
		return &m_pTextureSRV[index];
	}
	BOOL AddTextureDDS(ID3D11Device *pDevice, LPCWSTR file, LPCWSTR alternateName = NULL);
	BOOL AddTextureWIC(ID3D11Device *pDevice, LPCWSTR file, LPCWSTR alternateName = NULL);

	~TextureClass();
};

#endif //_TEXTURECLASS_H_