#ifndef _CFPS_CONTROLER_H
#define _CFPS_CONTROLER_H


#include <Windows.h>
#include <stdio.h>
#include <mmsystem.h>//mmsystem.hを読み込む前にWindows.hを読み込む
#pragma comment(lib,"winmm.lib")

#ifdef _SCREEN_WRITE	//スクリーン上にFPSを書き込む場合
#include "../DXFONT/DX_font.h"
#endif

class CFPS_CONTROLER
{
private:

	const int second;//1秒
	const float FPS;//現在は16msで更新されるように設定
	const DWORD measure_point;//FPS計測間隔基準値

	HWND m_hwnd;//ウィンドウハンドル

	//分解能。分解能が同じでなければBeginとEndは行われない。
	//エラー回避の目的が大きい。
	UINT resolution;

	DWORD execLastTime;//最後にゲーム処理した時間
	DWORD fpsLastTime;//最後にFPSを計測した時間
	DWORD currentTime;//最新時間
	int countFrame;//ゲーム処理回数
	float countFPS;

	static CFPS_CONTROLER* fps_controler;

	void Begin(UINT period);//分解能を設定する
	void End();//分解能の解放

#ifdef _SCREEN_WRITE
	DX_FONT dx_font;//フォント出力
#endif

public:
	
	explicit CFPS_CONTROLER(HWND hwnd);
	explicit CFPS_CONTROLER(HWND hwnd,UINT period);
	~CFPS_CONTROLER();

	static void AutoFPSControler(HWND hwnd);
	static void AutoFPSControler(HWND hwnd,UINT period);
	static void ReleaseFPSControler();
	static CFPS_CONTROLER* GetFPSControler();

	void getTime();//時間を取得し最新時間にセットする
	bool checkTime();//処理履行条件の確認
	void updateTime();//ゲーム処理回数と処理実行時間の保存
	void Measurement_FPS();//FPSの計測用関数

	//bool operator()(CONTROLER controler);

	void Draw();//FPSの出力

};

//マクロ関数定義
//-------------------------------------------------------------------------------
//FPS_CONTROLERのインターフェース使用時
inline CFPS_CONTROLER* GetFPSControler(){return CFPS_CONTROLER::GetFPSControler();};
//getTimeマクロ
inline void FPS_GetTime(){ GetFPSControler()->getTime(); };
//checktimeマクロ
inline bool FPS_CheckTime(){ return GetFPSControler()->checkTime(); };
//updatetimeマクロ
inline void FPS_UpdateTime(){ GetFPSControler()->updateTime(); };
//Measurement_FPSマクロ
inline void FPS_Measurement(){ GetFPSControler()->Measurement_FPS(); };
//FPSDrawマクロ
inline void FPS_Draw(){ GetFPSControler()->Draw(); }
//-------------------------------------------------------------------------------

#endif