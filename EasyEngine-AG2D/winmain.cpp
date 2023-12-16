
#include <sstream>
#include "winmain.h"
#include "EasyEngineAG2D.h"

// macro to read the key state
#define KEY_DOWN(vk) ((GetAsyncKeyState(vk) & 0x8000) ? 1 : 0)

HINSTANCE g_hInstance;
HWND g_hWnd;
int g_nCmdShow;

// Declare global Engine object

EE::Engine* g_engine;

bool gameover;

// Window Event callback function
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg)
	{
	case WM_QUIT:
	case WM_CLOSE:
	case WM_DESTROY:
		gameover = true;
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	MSG msg;
	srand((unsigned int)time(NULL));
	g_hInstance = hInstance;
	g_nCmdShow = nCmdShow;
	DWORD dwStyle, dwExStyle;
	RECT windowRect;

	/**
	 *  Create Engine object first
	 */
	g_engine = new EE::Engine();

	// Let main program have a crack at it before opening the window
	if (!game_preload()) {
		MessageBox(g_hWnd, "Error in Game preload!", "Error!", MB_OK);
		return 0;
	}

	// Get Window Caption string from engine
	char title[255];
	sprintf_s(title, "%s", g_engine->getAppTitle().c_str());

	// Set Window Dimension

	windowRect.left = (long)0;
	windowRect.right = (long)g_engine->getScreenWidth();
	windowRect.top = (long)0;
	windowRect.bottom = (long)g_engine->getScreenHeight();

	// Create Window class structure

	WNDCLASSEX wcx;

	// fill the struct with info
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = (WNDPROC)WinProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = NULL;
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = NULL;
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = title;
	wcx.hIconSm = NULL;

	// Set up window with class info
	RegisterClassEx(&wcx);

	// Set up the screen in windowed or full screen mode
	
	if (g_engine->getFullScreen()) {
		
		DEVMODE dm;
		memset(&dm, 0, sizeof(dm));
		dm.dmSize = sizeof(dm);
		dm.dmPelsWidth = g_engine->getScreenWidth();
		dm.dmPelsHeight = g_engine->getScreenHeight();
		dm.dmBitsPerPel = g_engine->getColorDepth();
		dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			MessageBox(NULL, "Display Mode Failed", NULL, MB_OK);
			g_engine->setFullScreen(false);
		}

		dwStyle = WS_POPUP;
		dwExStyle = WS_EX_APPWINDOW;
		ShowCursor(false);
	}
	else
	{
		dwStyle = WS_OVERLAPPEDWINDOW;
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	}

	// adjust window to true requested size
	AdjustWindowRectEx(&windowRect, dwStyle, NULL, dwExStyle);

	// Create the program window
	g_hWnd = CreateWindowEx(0,
		title,												//window class
		title,												//title bar
		dwStyle|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,			// window style
		0,0,												// X and Y coordinate
		windowRect.right - windowRect.left,					//width of the window
		windowRect.bottom - windowRect.top,					// Height of the window
		0,													// Parent window
		0,													// Menu
		g_hInstance,										// Application Instance
		0													// window parameter
	);

	// if error creating window
	if (!g_hWnd) {
		MessageBox(g_hWnd, "Error creating program window!", "Error!", MB_OK);
		return 0;
	}

	// Display the window
	ShowWindow(g_hWnd, g_nCmdShow);
	UpdateWindow(g_hWnd);

	// Initialize the Engine

	g_engine->setWindowHandle(g_hWnd);

	if (!g_engine->Init(g_engine->getScreenWidth(), g_engine->getScreenHeight(), g_engine->getColorDepth(), g_engine->getFullScreen())) {
		MessageBox(g_hWnd, "Error initializing the engine", "Error!", MB_OK);
		return 0;
	}

	// Main message loop
	gameover = false;
	while (!gameover) {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		g_engine->Update();
	}

	if (g_engine->getFullScreen()) {
		ShowCursor(true);
	}

	g_engine->Close();
	delete g_engine;

	return 1;
}