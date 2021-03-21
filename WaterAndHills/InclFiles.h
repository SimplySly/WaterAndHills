#ifndef _INCLFILES_H_
#define _INCLFILES_H_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <dxgi.h>
#include <Windows.h>
#include <vector>
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>
#include <string>
#include <fstream>
#include <map>
#include <unordered_map>

#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace std;

#define SafeRelease(object) object->Release(); object = nullptr
#define CheckHR(hr) if(FAILED(hr)) return 1

#if defined(_DEBUG) || defined(DEBUG)

#define f3string(a) (to_wstring(a.x) + L" " + to_wstring(a.y) + L" " + to_wstring(a.z) + L"\n\n")

#endif

#endif //_INCLFILES_H_