#pragma once  
#include <functional>
#include <string>
#include <vector>
#include <format>

class SimpleWindow  
{  
public:

    using WmCreateFunc = std::function<void(HWND)>;
	using WMCommandFunc = std::function<void(HWND, WPARAM)>;
	using WMPaintFunc = std::function<void(HWND)>;

    /// <summary>
    /// パラメータは全て、CreateWindowExの同名パラメータ
    /// クラス名はインスタンスごとに一意のものを使用する
    /// </summary>
    SimpleWindow(
        DWORD dwExStyle,
        LPCWSTR lpWindowName,
        DWORD dwStyle,
        int X,
        int Y,
        int nWidth,
        int nHeight,
        HWND hWndParent,
        HINSTANCE hInstance)
        : m_dwExStyle(dwExStyle),
        m_lpWindowName(lpWindowName),
        m_dwStyle(dwStyle),
        m_X(X),
        m_Y(Y),
        m_nWidth(nWidth),
        m_nHeight(nHeight),
        m_hWndParent(hWndParent),
        m_hInstance(hInstance)
    {
        m_ClassName = std::format(L"SimpleWindow{}", static_cast<void*>(this));
    }

    /// <summary>
    /// 呼び出し元で任意の処理を行うために、WM_CREATE発生時のイベントハンドラを登録する。
    /// nullptrなら呼び出さない。
    /// </summary>
    WmCreateFunc m_OnWmCreate = nullptr;
    /// <summary>
    /// 呼び出し元で任意の処理を行うために、WM_COMMAND発生時のイベントハンドラを登録する。
    /// nullptrなら呼び出さない。
    /// </summary>
    WMCommandFunc m_OnWmCommand = nullptr;
    /// <summary>
    /// 呼び出し元で任意の処理を行うために、WM_PAINT発生時のイベントハンドラを登録する。
    /// nullptrなら呼び出さない。
    /// </summary>
	WMPaintFunc m_OnWmPaint = nullptr;

	/// <summary>
	/// WM_CREATE等で作成した、このウインドウに所属するハンドルを登録する。登録するとこのインスタンスが所有している扱いになり、ウインドウクローズ時にまとめては記する。
	/// </summary>
	std::vector<HWND> m_WindowsPartsHandles;

    /// <summary>
    /// WM_CREATE等で作成した、このウインドウに所属するハンドルを登録する。登録するとこのインスタンスが所有している扱いになり、ウインドウクローズ時にまとめては記する。
    /// </summary>
    std::vector<HFONT> m_FontsHandles;

    /// <summary>
    /// ウインドウを開く。インスタンスごとに一度だけ使用可能。
    /// </summary>
    void WindowOpenAsync();

    /// <summary>
    /// 開いたウインドウを閉じる。
    /// </summary>
    void WindowClose();

private:
    void WindowThread();

    static LRESULT CALLBACK WndProcStatic(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    void OnWmCreate(HWND hWnd) const;
    void OnWmCommand(HWND hWnd, WPARAM wParam) const;
    void OnWmPaint(HWND hWnd) const;
    LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    DWORD m_dwExStyle;
	std::wstring m_ClassName;
    LPCWSTR m_lpWindowName;
    DWORD m_dwStyle;
    int m_X;
    int m_Y;
    int m_nWidth;
    int m_nHeight;
    HWND m_hWndParent;
    HINSTANCE m_hInstance;

    HWND m_hWnd = nullptr;

};
