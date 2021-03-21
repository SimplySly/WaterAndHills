#include "DxManager.h"


#define lpTitle L"Water And Hills"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


DxManager dx;


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		dx.m_Input->KeyDown(wParam);
		if (wParam == VK_ESCAPE)
		{
			ShowCursor(TRUE);
			int ret;

			ret = MessageBox(hwnd, L"Do you really want to exit?", L"Confirm Exit", MB_YESNO);

			if (ret == IDYES)
			{
				DestroyWindow(hwnd);
				PostQuitMessage(0);
			}
			ShowCursor(FALSE);
		}
		break;
	case WM_KEYUP:
		dx.m_Input->KeyUp(wParam);
		break;
	case WM_ACTIVATE:
		if (wParam == WA_INACTIVE)
		{
			dx.m_Input->ResetKeys();
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	HWND hw;
	WNDCLASSEX wc;
	LPWSTR lpClsName = L"Le Class";
	MSG msg;

	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = lpClsName;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	RegisterClassEx(&wc);

	hw = CreateWindowEx(0, lpClsName, lpTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, SCREEN_WIDTH, SCREEN_HEIGHT, nullptr, nullptr, hInst, nullptr);

	if (!hw)
	{
		return 1;
	}

	//Init DxClass
	if (dx.InitInstance(hw))
	{
		return 2;
	}

	SetCursorPos(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);

	POINT MouseLoc;

	GetCursorPos(&MouseLoc);
	ScreenToClient(hw, &MouseLoc);
	dx.m_Input->SetMouseLoc((float)MouseLoc.x, (float)MouseLoc.y);

	ShowWindow(hw, SW_SHOW);

	ZeroMemory(&msg, sizeof(msg));

	bool done = false;

	while (!done)
	{
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				done = true;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//render
		dx.Update();
		dx.Render();
	}

	return msg.wParam;
}