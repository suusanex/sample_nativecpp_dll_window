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
    /// �p�����[�^�͑S�āACreateWindowEx�̓����p�����[�^
    /// �N���X���̓C���X�^���X���ƂɈ�ӂ̂��̂��g�p����
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
    /// �Ăяo�����ŔC�ӂ̏������s�����߂ɁAWM_CREATE�������̃C�x���g�n���h����o�^����B
    /// nullptr�Ȃ�Ăяo���Ȃ��B
    /// </summary>
    WmCreateFunc m_OnWmCreate = nullptr;
    /// <summary>
    /// �Ăяo�����ŔC�ӂ̏������s�����߂ɁAWM_COMMAND�������̃C�x���g�n���h����o�^����B
    /// nullptr�Ȃ�Ăяo���Ȃ��B
    /// </summary>
    WMCommandFunc m_OnWmCommand = nullptr;
    /// <summary>
    /// �Ăяo�����ŔC�ӂ̏������s�����߂ɁAWM_PAINT�������̃C�x���g�n���h����o�^����B
    /// nullptr�Ȃ�Ăяo���Ȃ��B
    /// </summary>
	WMPaintFunc m_OnWmPaint = nullptr;

	/// <summary>
	/// WM_CREATE���ō쐬�����A���̃E�C���h�E�ɏ�������n���h����o�^����B�o�^����Ƃ��̃C���X�^���X�����L���Ă��鈵���ɂȂ�A�E�C���h�E�N���[�Y���ɂ܂Ƃ߂Ă͋L����B
	/// </summary>
	std::vector<HWND> m_WindowsPartsHandles;

    /// <summary>
    /// WM_CREATE���ō쐬�����A���̃E�C���h�E�ɏ�������n���h����o�^����B�o�^����Ƃ��̃C���X�^���X�����L���Ă��鈵���ɂȂ�A�E�C���h�E�N���[�Y���ɂ܂Ƃ߂Ă͋L����B
    /// </summary>
    std::vector<HFONT> m_FontsHandles;

    /// <summary>
    /// �E�C���h�E���J���B�C���X�^���X���ƂɈ�x�����g�p�\�B
    /// </summary>
    void WindowOpenAsync();

    /// <summary>
    /// �J�����E�C���h�E�����B
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
