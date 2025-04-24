#include "pch.h"
#include "SimpleWindow.h"

#include <string>
#include <thread>


LRESULT CALLBACK SimpleWindow::WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    SimpleWindow* pThis = nullptr;

    if (message == WM_NCCREATE) {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = static_cast<SimpleWindow*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else {
        pThis = reinterpret_cast<SimpleWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (pThis) {
        return pThis->WndProc(hWnd, message, wParam, lParam);
    }
    else
    {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}


void SimpleWindow::OnWmCreate(HWND hWnd) const
{
	if (m_OnWmCreate)
	{
		m_OnWmCreate(hWnd);
	}
}

void SimpleWindow::OnWmCommand(HWND hWnd, WPARAM wParam) const
{
	if (m_OnWmCommand) {
		m_OnWmCommand(hWnd, wParam);
	}
}

void SimpleWindow::OnWmPaint(HWND hWnd) const
{
    if (m_OnWmPaint)
    {
		m_OnWmPaint(hWnd);
	}
}

LRESULT SimpleWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        OnWmCreate(hWnd);
        return 0;

    case WM_COMMAND:
        OnWmCommand(hWnd, wParam);
        return 0;

    case WM_PAINT: {
        OnWmPaint(hWnd);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void SimpleWindow::WindowThread(){
	try
	{
		WNDCLASS wc = {};
		wc.lpfnWndProc = WndProcStatic;
		wc.hInstance = m_hInstance;
		wc.lpszClassName = m_ClassName.c_str();
		if (RegisterClass(&wc) == 0)
		{
			const auto win32Err = GetLastError();
			throw std::exception(("RegisterClass Fail, " + std::to_string(win32Err)).c_str());
		}

		m_hWnd = CreateWindowEx(
			m_dwExStyle, m_ClassName.c_str(), m_lpWindowName,
			m_dwStyle, m_X, m_Y, m_nWidth, m_nHeight,
			m_hWndParent, nullptr, m_hInstance, this);

		if (!m_hWnd)
		{
			const auto win32Err = GetLastError();
			throw std::exception(("CreateWindowEx Fail, " + std::to_string(win32Err)).c_str());			
		}

		ShowWindow(m_hWnd, SW_SHOW);


		MSG msg = {};
		while (GetMessage(&msg, nullptr, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	catch (std::exception& e)
	{
		OutputDebugStringA(e.what());
	}

}

void SimpleWindow::WindowOpenAsync()
{
	std::thread th(&SimpleWindow::WindowThread, this);
	th.detach();

}

void SimpleWindow::WindowClose()
{
	if (m_hWnd)
	{
		if (!PostMessage(m_hWnd, WM_CLOSE, 0, 0))
		{
			const auto win32Err = GetLastError();
			throw std::exception(("PostMessage Fail, " + std::to_string(win32Err)).c_str());
		}
		m_hWnd = nullptr;
	}
	for (auto& handle : m_WindowsPartsHandles)
	{
		if (handle)
		{
			if (!PostMessage(handle, WM_CLOSE, 0, 0))
			{
				const auto win32Err = GetLastError();
				throw std::exception(("PostMessage Fail, " + std::to_string(win32Err)).c_str());
			}

			handle = nullptr;
		}
	}
	m_WindowsPartsHandles.clear();
	for (auto& handle : m_FontsHandles)
	{
		if (handle)
		{
			DeleteObject(handle);
			handle = nullptr;
		}
	}
}

