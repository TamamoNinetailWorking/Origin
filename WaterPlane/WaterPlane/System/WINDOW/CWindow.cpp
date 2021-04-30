#include "CWindow.h"

HRESULT CWINDOW::WNDC_initialize(const HINSTANCE hInstance,const LPCSTR name) 
{
	WNDCLASSEX wcex;
	wcex.hInstance = hInstance;//インスタンス値のセット
	wcex.lpszClassName = name;//クラス名
	wcex.lpfnWndProc = (WNDPROC)WndProc;//ウィンドウメッセージ
	wcex.style = 0;//ウィンドウスタイル
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hIcon = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);//ラージアイコン
	wcex.hIconSm = LoadIcon((HINSTANCE)NULL, IDI_WINLOGO);//スモールアイコン
	wcex.hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);//カーソルスタイル
	wcex.lpszMenuName = 0;//メニューなし
	wcex.cbClsExtra = 0;//エキストラなし
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);//背景色白

	if (!RegisterClassEx(&wcex)) {
		return FALSE;
	}else{
		return S_OK;
	}
}

HRESULT CWINDOW::HWND_initialize(const HINSTANCE hInstance, const LPCSTR name, const LPCSTR title,const bool full_screen)
{
	if (full_screen){//フルスクリーン
		m_hWnd = CreateWindow(
			name,//ウィンドウクラスの名前
			title,//タイトル
			WS_VISIBLE | WS_POPUP,//ウィンドウスタイル
			0, 0,//ウィンドウ位置 縦、横
			m_width, m_height,//ウィンドウサイズ
			NULL,//親ウィンドウなし
			(HMENU)NULL,//メニューなし
			hInstance,//インスタンスハンドル
			(LPVOID)NULL//追加引数なし
		);
	}//フルスクリーンEnd
	else 
	{//ウィンドウモード
		RECT rWindow, rClient;
		int width = m_width, height = m_height;

		m_hWnd = CreateWindow(
			name,//ウィンドウクラスの名前
			title,//タイトル
			WS_CAPTION | WS_SYSMENU,//ウィンドウスタイル
			0, 0,//ウィンドウ位置 縦、横（あとで中央に移動させる
			m_width, m_height,//ウィンドウサイズ
			HWND_DESKTOP,//親ウィンドウなし
			(HMENU)NULL,//メニューなし
			hInstance,//インスタンづはんどる
			(LPVOID)NULL//追加引数なし
		);

		//ウィンドウサイズを再計算(Metricsだけではフレームデザインとクライアント領域サイズが変わってしまう）
		GetWindowRect(m_hWnd, &rWindow);
		GetClientRect(m_hWnd, &rClient);
		width = (rWindow.right - rWindow.left) - (rClient.right - rClient.left) + m_width;
		height = (rWindow.bottom - rWindow.top) - (rClient.bottom - rClient.top) + m_height;
		SetWindowPos(
			m_hWnd,
			NULL,
			GetSystemMetrics(SM_CXSCREEN)/2-width/2,
			GetSystemMetrics(SM_CYSCREEN)/2-height/2,
			width-1,
			height-1,
			SWP_NOZORDER
		);
	}//ウィンドウモードEnd

	if (!m_hWnd) {
		return FALSE;
	}else {
		return S_OK;
	}
}

HRESULT CWINDOW::Create_Window(const HINSTANCE hInstance, const int nWindMode)
{
	m_hInstance = hInstance;

	//ウィンドウクラスの登録
	if (FAILED(WNDC_initialize(hInstance, m_window_name)))
	{
		MessageBox(m_hWnd, "ERROR!", "WNDC Error", MB_OK);
		return FALSE;
	}

	//ウィンドウハンドルの登録
	if (FAILED(HWND_initialize(hInstance, m_window_name, m_window_title,m_full_screen_flag)))
	{
		MessageBox(m_hWnd, "ERROR!", "HWND Error", MB_OK);
		return FALSE;
	}
	CreateConsoleWindow();
	ShowWindow(m_hWnd, nWindMode);
	UpdateWindow(m_hWnd);
	return S_OK;
}

