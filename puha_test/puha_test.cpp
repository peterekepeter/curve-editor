// puha_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int windowWidth = 0;
int windowHeight = 0;

class PuhaWin32Outputter
{
	// owned resources
	HBITMAP map;
	HDC src;

	int width = 320;
	int height = 200;

	// referenced resources
	Gfx320x200& gfx;
	HDC hdc;
public:
	PuhaWin32Outputter(Gfx320x200& gfx, HDC hdc) : gfx(gfx), hdc(hdc) {
		// Creating temp bitmap
		map = CreateBitmap(width, // width. 512 in my case
			height, // height
			1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
			8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
			(void*)gfx.GetBuffer()); // pointer to array


		src = CreateCompatibleDC(hdc); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
		SelectObject(src, map); // Inserting picture into our temp HDC
								// Copy image from temp HDC to window
	}

	void Blit() {
		auto result = StretchBlt(hdc, // Destination
			0,  // x and
			0,  // y - upper-left corner of place, where we'd like to copy
			windowWidth, // width of the region
			windowHeight, // height
			src, // source
			0,   // x and
			0,
			width,
			height,// y of upper left corner  of part of the source, from where we'd like to copy
			SRCCOPY); // Defined DWORD to juct copy pixels. Watch more on msdn;
	}

	~PuhaWin32Outputter() {
		DeleteObject(map);
		DeleteDC(src); // Deleting temp HDC
	}

	HDC GetHDC() {
		return hdc;
	}
};

Gfx320x200 gfx;

void Repaint(HWND hWnd) {
	PAINTSTRUCT ps;
	auto hdc = BeginPaint(hWnd, &ps);
	{
		PuhaWin32Outputter outputter(gfx, hdc);
		outputter.Blit();
	}
	EndPaint(hWnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Hello, World!");

	switch (message)
	{
	case WM_PAINT:
		Repaint(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		windowWidth = LOWORD(lParam);
		windowHeight = HIWORD(lParam);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

const wchar_t* szWindowClass = L"MyWindowClass";
const wchar_t* szTitle = L"Test";

int main()
{
	HINSTANCE hInstance = GetModuleHandle(nullptr);
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	RECT rect = { 0, 0, 320 * 3, 200 * 3 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top,
		NULL, 
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Win32 Guided Tour"),
			NULL);

		return 1;
	}

	// draw something
	gfx.SetColor(0);
	gfx.RectangleFilled(0, 0, 319, 199);
	gfx.SetColor(0xffffff);
	gfx.Line(0, 0, 64, 64);

	// The parameters to ShowWindow explained:  
	// hWnd: the value returned from CreateWindow  
	// nCmdShow: the fourth parameter from WinMain  
	ShowWindow(hWnd,
		true);
	UpdateWindow(hWnd);
	
	// Main message loop:  
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

