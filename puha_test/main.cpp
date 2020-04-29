// puha_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../puha-render/puha-render.h"
#include "Application.h"

int windowWidth = 0;
int windowHeight = 0;
std::unique_ptr<Application> app = nullptr;

Gfx320x200 gfx;
int gfx_buffer[320 * 200];

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
			(void*)gfx_buffer); // pointer to array


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

void Repaint(HWND hWnd) {
	PAINTSTRUCT ps;
	auto hdc = BeginPaint(hWnd, &ps);
	{
		PuhaWin32Outputter outputter(gfx, hdc);
		outputter.Blit();
	}
	EndPaint(hWnd, &ps);
}


bool mod_shift = false;
bool mod_ctrl = false;
bool mod_alt = false;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	bool keyDown = true;
	bool m_left_button_down = false;
	bool m_left_button_up = false;
	switch (message)
	{
	case WM_MOUSEWHEEL: {
		short *posptr = (short*)&wParam, delta = posptr[1];
		if (delta > 0) {
			app->ZoomIn();
		}
		else {
			app->ZoomOut();
		}
		break;
	}
	case WM_RBUTTONUP:
		app->CancelCurrentEdit();
		break;
	case WM_LBUTTONUP: {
		m_left_button_up = true;
	case WM_LBUTTONDOWN:
		m_left_button_down = true;
	case WM_MOUSEMOVE: 
		short *posptr = (short*)&lParam, x_pos = posptr[0], y_pos = posptr[1];
		app->UpdateMousePos(((int)x_pos) * 320 / windowWidth, (((int)y_pos)*200) / windowHeight);
		if (m_left_button_down) app->UpdateLeftButton(true);
		if (m_left_button_up) app->UpdateLeftButton(false);
		break;
	}
	case WM_KEYUP:
		keyDown = false;
	case WM_KEYDOWN: {
		bool nextState = keyDown;
		// OnKeystateChange(int(wParam), keyDown);
		// on esc exit!
		if (wParam == VK_SHIFT) { mod_shift = nextState; }
		if (wParam == VK_CONTROL) { mod_ctrl = nextState; }
		if (wParam == VK_MENU) { mod_alt = nextState; }
		if (nextState == false) {
			break;
		}
		if (wParam == VK_RIGHT) { app->ShiftView(+1); }
		if (wParam == VK_LEFT) { app->ShiftView(-1); }
		if (wParam == 'E' || wParam == VK_TAB) { app->ToggleEditMode(); }
		if (wParam == VK_ESCAPE) { app->CancelCurrentEdit(); }
		if (mod_alt && wParam == VK_F4) { 
			DestroyWindow(hWnd); 
			break; // switch
		}

		if (wParam == 'T') {
			app->ChangeParamCount();
			break;
		}

		if (mod_ctrl && mod_shift && wParam == 'Y') {
			app->Undo(); 
			break; // switch
		}

		if (mod_ctrl && mod_shift && wParam == 'Z') {
			app->Redo(); 
			break; // switch
		}

		if (mod_ctrl && wParam == 'Y') {
			app->Redo(); 
			break; // switch
		}

		if (mod_ctrl && wParam == 'Z') {
			app->Undo(); 
			break; // switch
		}

		if (wParam == 'S') { app->SplitCurve(); }
		
		break;
	}
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

void entry()
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
			L"Call to RegisterClassEx failed!",
			L"Win32 Guided Tour",
			NULL);
		ExitProcess(1);
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
			L"Call to CreateWindow failed!",
			L"Win32 Guided Tour",
			NULL);
		ExitProcess(1);
	}

	// The parameters to ShowWindow explained:  
	// hWnd: the value returned from CreateWindow  
	// nCmdShow: the fourth parameter from WinMain  
	ShowWindow(hWnd,true);
	UpdateWindow(hWnd);
	bool finished = false;

	// mount the application
	app = std::make_unique<Application>(gfx);
	app->SetRedrawHandler([&] {
		memcpy(gfx_buffer, gfx.GetBuffer(), sizeof(int) * 320 * 200);
		RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_NOERASE | RDW_NOFRAME);
	});

	// Main message loop:  
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	finished = true;
	app.release();
	ExitProcess(msg.wParam);
}

int main() {
	entry();
}

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
) {
	entry();
}
