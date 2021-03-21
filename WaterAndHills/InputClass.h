#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

#include <Windows.h>
#include <math.h>
#include <string>

class FloatMouseMove
{
public:
	float MouseDX;
	float MouseDY;
	
	FloatMouseMove operator *= (float Scaler)
	{
		MouseDX *= Scaler;
		MouseDY *= Scaler;

		return *this;
	}
};

class InputClass
{
protected:
	HWND m_hwnd;
	bool m_key[256]; //true if key is pressed
	float m_MouseX, m_MouseY;
	float m_scrWidth, m_scrHeight;
public:

	InputClass(HWND hwnd);
	~InputClass();

	bool IsKeyDown(unsigned int key);
	FloatMouseMove GetMouseMove();
	
	void KeyDown(unsigned int key);
	void KeyUp(unsigned int key);
	void SetMouseLoc(float x, float y);
	void ResetKeys();
};



#endif