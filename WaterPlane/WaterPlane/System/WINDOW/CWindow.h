#pragma once

#include <Windows.h>

#pragma comment(lib,"winmm.lib")

class CWINDOW
{
private:

	const char* m_window_name;//ウィンドウ名
	const char* m_window_title;//ウィンドウタイトル
	const int m_timer_resolution;//タイマーの分解能
	const int m_frame_max;//フレーム最大値
	const int m_frame_per_second;//１フレームあたりの処理時間
	bool m_full_screen_flag;//フルスクリーン設定

	HWND m_hWnd;//ウィンドウハンドル
	MSG m_msg;//メッセージ
	int m_width;//ウィンドウの横幅
	int m_height;//ウィンドウの高さ
	HINSTANCE m_hInstance;//ウィンドウ構造体
	int m_timer_id;//タイマーID
	int m_hConsole;//コンソールウィンドウのハンドル

	HRESULT WNDC_initialize(const HINSTANCE hInstance,const LPCSTR name);
	HRESULT HWND_initialize(const HINSTANCE hInstance, const LPCSTR name, const LPCSTR title,const bool full_screen);

public:

	CWINDOW()//デフォルトコンストラクタ
		:m_window_name("WinApp"),
		m_window_title("Shader Tester"),
		m_full_screen_flag(false),
		m_width(1280),
		m_height(720),
		m_timer_resolution(1),
		m_frame_max(60),
		m_frame_per_second(1000/m_frame_max),
		m_hConsole(0){};//定数値の初期化
	~CWINDOW() {};//デストラクタ

	HRESULT Create_Window(const HINSTANCE hInstance, const int nWindMode);

	void CreateConsoleWindow();
	void CloseConsoleWindow();

	LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void Loop();//メッセージループ
	void App();//アプリケーションの本体部分処理

	HINSTANCE GetHinstance() { return m_hInstance; };
	HWND GetHWND() { return m_hWnd; };
	MSG GetMSG() { return m_msg; };
	int GetWindowWidth() { return m_width; };
	int GetWindowHeight() { return m_height; };

};

//外部参照
extern CWINDOW* g_pWindow;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);