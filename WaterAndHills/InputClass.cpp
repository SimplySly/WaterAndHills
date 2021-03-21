#include "InputClass.h"

////////////////////////////////////////////////////////////
//		Constructors And Destructors
//

InputClass::InputClass(HWND hwnd)
{
	m_hwnd = hwnd;

	RECT rc;

	GetWindowRect(hwnd, &rc);

	m_scrHeight = (float)(rc.bottom - rc.top);
	m_scrWidth = (float)(rc.right - rc.left);

	ResetKeys();
}

InputClass::~InputClass()
{

}

////////////////////////////////////////////////////////////
//		Public Functions
//

FloatMouseMove InputClass::GetMouseMove()
{
	POINT MouseLoc;
	FloatMouseMove result;

	GetCursorPos(&MouseLoc);
	ScreenToClient(m_hwnd, &MouseLoc);

	OutputDebugString((std::to_wstring(MouseLoc.x) + L" " + std::to_wstring(m_MouseX) + L"\n").c_str());
	result.MouseDX = MouseLoc.x - (float)m_MouseX;

	result.MouseDY = MouseLoc.y - (float)m_MouseY;

	
	SetCursorPos(m_scrWidth/2.f, m_scrHeight/2.f);

	return result;
}

void InputClass::SetMouseLoc(float x, float y)
{
	m_MouseX = x;
	m_MouseY = y;
}

void InputClass::KeyDown(unsigned int key)
{
	m_key[key] = true;
}

void InputClass::KeyUp(unsigned int key)
{
	m_key[key] = false;
}

bool InputClass::IsKeyDown(unsigned int key)
{
	return m_key[key];
}

void InputClass::ResetKeys()
{
	for (unsigned int i = 0; i < 256; i++)
	{
		m_key[i] = false;
	}
}