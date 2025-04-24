// dllmain.cpp : DLL アプリケーションのエントリ ポイントを定義します。
#include "pch.h"

#include <string>
#include <thread>

#include "resource.h"
#include "SimpleWindow.h"


HINSTANCE g_hInstance = nullptr;
HBITMAP g_hBitmap = nullptr;
SimpleWindow* g_window = nullptr;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
	case DLL_PROCESS_ATTACH:
		g_hInstance = hModule;
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        delete g_window;
        break;
    }
    return TRUE;
}

extern "C" __declspec(dllexport) void __stdcall TestWindowOpen()
{
	try
	{
		if (!g_hBitmap)
		{
			auto loadImageResult = LoadImageW(
				g_hInstance, MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0, 0, 0);
			if (!loadImageResult)
			{
				const auto win32Err = GetLastError();
				const auto errMsg = L"LoadImageW Fail, " + std::to_wstring(win32Err);
				MessageBoxW(nullptr, errMsg.c_str(), L"Error", MB_OK);
				return;
			}
			g_hBitmap = static_cast<HBITMAP>(loadImageResult);
		}

		if (!g_window)
		{
			g_window = new SimpleWindow(0, L"My Window",
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
				nullptr, g_hInstance);

			g_window->m_OnWmCreate = [](HWND hWnd) {
				auto hButton = CreateWindow(
					L"BUTTON", L"Click Me",
					WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
					50, 200, 100, 30,
					hWnd, (HMENU)1, g_hInstance, nullptr);
				g_window->m_WindowsPartsHandles.emplace_back(hButton);
			};

			g_window->m_OnWmCommand = [](HWND hWnd, WPARAM wParam) {
				if (LOWORD(wParam) == 1) {
					MessageBox(hWnd, L"Button Clicked!", L"Info", MB_OK);
				}
				};
			g_window->m_OnWmPaint = [](HWND hWnd) {

				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				if (g_hBitmap) {
					HDC hMemDC = CreateCompatibleDC(hdc);
					HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, g_hBitmap);
					BITMAP bmp;
					GetObject(g_hBitmap, sizeof(BITMAP), &bmp);
					BitBlt(hdc, 50, 50, bmp.bmWidth, bmp.bmHeight, hMemDC, 0, 0, SRCCOPY);
					SelectObject(hMemDC, hOldBitmap);
					DeleteDC(hMemDC);
				}
				EndPaint(hWnd, &ps);
				};


			g_window->WindowOpenAsync();
		}
        
	}
	catch (std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Error", MB_OK);
	}
	catch (...)
	{
		MessageBoxW(nullptr, L"Unknown error", L"Error", MB_OK);
	}

	return;
}

extern "C" __declspec(dllexport) void __stdcall TestWindowClose()
{
	try
	{
		if (!g_window) return;

		g_window->WindowClose();
		delete g_window;
		g_window = nullptr;


	}
	catch (std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Error", MB_OK);
	}
	catch (...)
	{
		MessageBoxW(nullptr, L"Unknown error", L"Error", MB_OK);
	}
	
}
