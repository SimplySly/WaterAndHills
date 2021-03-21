#include "DxManager.h"

////////////////////////////////////////////////////////////
//		Constructors And Destructors
//

DxManager::DxManager()
{
	m_pVertexBuffer = nullptr;
	m_pMatrixBuffer = nullptr;
	m_pIndexBuffer = nullptr;

	m_pBasicShader = nullptr;

	m_pDxStructs = nullptr;
}


DxManager::~DxManager()
{
	if (m_pVertexBuffer)
		SafeRelease(m_pVertexBuffer);
	if (m_pIndexBuffer)
		SafeRelease(m_pIndexBuffer);
	if (m_pMatrixBuffer)
		SafeRelease(m_pMatrixBuffer);

	if (m_pBasicShader)
		delete m_pBasicShader;

	if (m_pDxStructs)
		delete m_pDxStructs;
}


////////////////////////////////////////////////////////////
//		Public Functions
//

void DxManager::Update()
{
	
	//////////////////////////////////////
	//              CAMERA
	if (GetActiveWindow() == m_hwnd)
	{
		FloatMouseMove dMouse = m_Input->GetMouseMove();
		float MoveForward = 0.f;
		float Strafe = 0.f;

		if (m_Input->IsKeyDown('W'))
		{
			MoveForward += 0.05f;
		}
		if (m_Input->IsKeyDown('S'))
		{
			MoveForward += -0.05f;
		}
		if (m_Input->IsKeyDown('A'))
		{
			Strafe += 0.05f;
		}
		if (m_Input->IsKeyDown('D'))
		{
			Strafe += -0.05f;
		}

		dMouse *= 0.1f;

		m_Camera.MoveCamera(MoveForward, Strafe);
		m_Camera.RotateCamera(dMouse.MouseDY, dMouse.MouseDX, 0.f);
		m_Camera.UpdateCamera();
	}

	//////////////////////////////////////
	//              MODELS

	m_WaterTrans += 0.003f;
}

void DxManager::Render()
{
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, Colors::Silver);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	float blendfactor[4] = { 0.f };
	UINT stride = sizeof(Vertex), offset = 0;
	
	m_pDeviceContext->RSSetState(m_pDxStructs->m_pBackCullRS);
	m_pDeviceContext->OMSetBlendState(m_pDxStructs->m_pTransparentBlender, blendfactor, 0xFFFFFFFF);
	m_pDeviceContext->PSSetSamplers(0, 1, &m_pDxStructs->m_pLinearWrapSampler);
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pMatrixBuffer);

	//Draw terrain
	m_pDeviceContext->PSSetShader(m_pBasicShader->m_pPixelShader, NULL, 0);
	m_pDeviceContext->VSSetShader(m_pBasicShader->m_pVertexShader, NULL, 0);
	m_pDeviceContext->IASetInputLayout(m_pBasicShader->m_pInputLayout);
	m_pDeviceContext->PSSetShaderResources(0, 1, m_Textures[L"grass"]);

	m_world = XMMatrixIdentity();
	UpdateMatrixBuffer(m_world, m_Camera.GetCamera(), m_projection);
	m_pDeviceContext->DrawIndexed(m_IndexCount[0], 0, 0);

	//Draw water -- needs to be second because of blending
	m_pDeviceContext->PSSetShader(m_pWaterShader->m_pPixelShader, NULL, 0);
	m_pDeviceContext->VSSetShader(m_pWaterShader->m_pVertexShader, NULL, 0);
	m_pDeviceContext->IASetInputLayout(m_pWaterShader->m_pInputLayout);
	m_pDeviceContext->PSSetShaderResources(0, 1, m_Textures[L"water2"]);

	m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pWaterConstBuffer);
	UpdateWaterConstBuffer(XMMatrixTranslation(m_WaterTrans, 0.f, 0.f));

	m_pDeviceContext->DrawIndexed(m_IndexCount[0], 0, m_VertexCount[0]);
	
	m_pSwapChain->Present(1, 0);
}

BOOL DxManager::InitInstance(HWND hw)
{
	UINT MSAA_Quality, MSAA_Count = 4;

	ShowCursor(FALSE);

	m_hwnd = hw;
	m_Input = new InputClass(hw);
	m_WaterTrans = 0.f;

	if (InitDeviceAndSwapChain(MSAA_Count, &MSAA_Quality, nullptr, TRUE))
	{
		return 1;
	}

	if (InitRenderTargets(MSAA_Count, MSAA_Quality))
	{
		return 1;
	}

	InitViewPort();

	BOOL result;
	D3D11_INPUT_ELEMENT_DESC BasicIED[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	m_pBasicShader = new ShaderClass();
	result = m_pBasicShader->InitShaderCode(m_pDevice, BasicIED, ARRAYSIZE(BasicIED), L"Shaders\\BasicVertex.hlsl", L"Shaders\\BasicPixel.hlsl");
	if (result)
	{
		return 1;
	}

	D3D11_INPUT_ELEMENT_DESC WaterIED[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	m_pWaterShader = new ShaderClass();
	result = m_pWaterShader->InitShaderCode(m_pDevice, WaterIED, ARRAYSIZE(WaterIED), L"Shaders\\WaterVertex.hlsl", L"Shaders\\WaterPixel.hlsl");
	if (result)
	{
		return 1;
	}

	m_pDxStructs = new DxStructs();
	if (m_pDxStructs->InitStructs(m_pDevice))
	{
		return 1;
	}

	if (InitBuffers())
	{
		return 1;
	}

	//init matrices
	m_projection = XMMatrixPerspectiveFovLH(XM_PI / 4.f, (float)m_ScrWidth / (float)m_ScrHeight, 0.1f, 100.f);
	m_Camera.SetCameraDefaultValue(XMFLOAT3(0.f, 0.3f, -1.f), XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(0.f, 1.f, 0.f));

	//load textures
	if (LoadTextures("Textures.txl"))
	{
		return 1;
	}

	return 0;
}

////////////////////////////////////////////////////////////
//		Protected Functions
//

BOOL DxManager::InitBuffers()
{
	HRESULT hr;
	D3D11_BUFFER_DESC vd, id, md, wd;
	D3D11_SUBRESOURCE_DATA vsubr, isubr;

	vector<Vertex> vertices, buffer;
	vector<UINT> indices, ibuffer;

	vector<Vertex>::iterator itv = vertices.begin();

	//hills
	buffer = GenerateHills(10.f, 10.f, indices, 4);
	m_VertexCount.push_back(buffer.size());
	m_IndexCount.push_back(indices.size());
	
	vertices.reserve(vertices.size() + buffer.size());
	copy(buffer.begin(), buffer.end(), back_inserter(vertices));

	//water
	buffer = GenerateGrid(10.f, 10.f, indices, 4);
	m_VertexCount.push_back(buffer.size());
	m_IndexCount.push_back(indices.size());

	vertices.reserve(vertices.size() + buffer.size());
	copy(buffer.begin(), buffer.end(), back_inserter(vertices));

	ZeroMemory(&vd, sizeof(vd));
	vd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vd.ByteWidth = sizeof(Vertex) * vertices.size();
	vd.Usage = D3D11_USAGE_DEFAULT;

	ZeroMemory(&vsubr, sizeof(vsubr));
	vsubr.pSysMem = vertices.data();

	hr = m_pDevice->CreateBuffer(&vd, &vsubr, &m_pVertexBuffer);
	CheckHR(hr);

	
	ZeroMemory(&id, sizeof(id));
	id.BindFlags = D3D11_BIND_INDEX_BUFFER;
	id.ByteWidth = sizeof(UINT) * indices.size();
	id.Usage = D3D11_USAGE_DEFAULT;

	ZeroMemory(&isubr, sizeof(isubr));
	isubr.pSysMem = indices.data();

	hr = m_pDevice->CreateBuffer(&id, &isubr, &m_pIndexBuffer);
	CheckHR(hr);
	

	ZeroMemory(&md, sizeof(md));
	md.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	md.ByteWidth = sizeof(MatrixData);
	md.Usage = D3D11_USAGE_DEFAULT;

	hr = m_pDevice->CreateBuffer(&md, nullptr, &m_pMatrixBuffer);
	CheckHR(hr);

	ZeroMemory(&wd, sizeof(wd));
	wd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	wd.ByteWidth = sizeof(WaterData);
	wd.Usage = D3D11_USAGE_DEFAULT;

	hr = m_pDevice->CreateBuffer(&wd, nullptr, &m_pWaterConstBuffer);
	CheckHR(hr);

	return 0;
}

void DxManager::UpdateMatrixBuffer(CXMMATRIX World, CXMMATRIX View, CXMMATRIX Projection)
{
	MatrixData md;

	XMStoreFloat4x4(&md.world, XMMatrixTranspose(World));
	XMStoreFloat4x4(&md.view, XMMatrixTranspose(View));
	XMStoreFloat4x4(&md.projection, XMMatrixTranspose(Projection));

	m_pDeviceContext->UpdateSubresource(m_pMatrixBuffer, 0, nullptr, &md, 0, 0);
}

void DxManager::UpdateWaterConstBuffer(CXMMATRIX WaterTexMove)
{
	WaterData wd;

	XMStoreFloat4x4(&wd.WaterTexMove, XMMatrixTranspose(WaterTexMove));

	m_pDeviceContext->UpdateSubresource(m_pWaterConstBuffer, 0, nullptr, &wd, 0, 0);
}

BOOL DxManager::LoadTextures(string texFile)
{
	string line;
	fstream fileTex;

	fileTex.open(texFile, ios::in);

	if (fileTex.fail())
	{
		return 1;
	}
	
	while (getline(fileTex, line))
	{
		if (line[0] == '/' && line[1] == '/' || line == "")
		{
			continue;
		}

		wstring filename(&line[0], &line[line.size()]);

		if (line.substr(line.size()-4, 4) == ".dds")
		{
			wstring alt(&line[line.find_last_of('\\') + 1], &line[line.find_last_of('.')]);
			if (m_Textures.AddTextureDDS(m_pDevice, filename.data(), alt.data()))
			{
				return 1;
			}
		}
		else
		{
			wstring alt(&line[line.find_last_of('\\') + 1], &line[line.find_last_of('.')]);
			if (m_Textures.AddTextureWIC(m_pDevice, filename.data(), alt.data()))
			{
				return 1;
			}
		}
	}

	return 0;
}

