#ifndef _CFPS_CONTROLER_H
#define _CFPS_CONTROLER_H


#include <Windows.h>
#include <stdio.h>
#include <mmsystem.h>//mmsystem.h��ǂݍ��ޑO��Windows.h��ǂݍ���
#pragma comment(lib,"winmm.lib")

#ifdef _SCREEN_WRITE	//�X�N���[�����FPS���������ޏꍇ
#include "../DXFONT/DX_font.h"
#endif

class CFPS_CONTROLER
{
private:

	const int second;//1�b
	const float FPS;//���݂�16ms�ōX�V�����悤�ɐݒ�
	const DWORD measure_point;//FPS�v���Ԋu��l

	HWND m_hwnd;//�E�B���h�E�n���h��

	//����\�B����\�������łȂ����Begin��End�͍s���Ȃ��B
	//�G���[����̖ړI���傫���B
	UINT resolution;

	DWORD execLastTime;//�Ō�ɃQ�[��������������
	DWORD fpsLastTime;//�Ō��FPS���v����������
	DWORD currentTime;//�ŐV����
	int countFrame;//�Q�[��������
	float countFPS;

	static CFPS_CONTROLER* fps_controler;

	void Begin(UINT period);//����\��ݒ肷��
	void End();//����\�̉��

#ifdef _SCREEN_WRITE
	DX_FONT dx_font;//�t�H���g�o��
#endif

public:
	
	explicit CFPS_CONTROLER(HWND hwnd);
	explicit CFPS_CONTROLER(HWND hwnd,UINT period);
	~CFPS_CONTROLER();

	static void AutoFPSControler(HWND hwnd);
	static void AutoFPSControler(HWND hwnd,UINT period);
	static void ReleaseFPSControler();
	static CFPS_CONTROLER* GetFPSControler();

	void getTime();//���Ԃ��擾���ŐV���ԂɃZ�b�g����
	bool checkTime();//�������s�����̊m�F
	void updateTime();//�Q�[�������񐔂Ə������s���Ԃ̕ۑ�
	void Measurement_FPS();//FPS�̌v���p�֐�

	//bool operator()(CONTROLER controler);

	void Draw();//FPS�̏o��

};

//�}�N���֐���`
//-------------------------------------------------------------------------------
//FPS_CONTROLER�̃C���^�[�t�F�[�X�g�p��
inline CFPS_CONTROLER* GetFPSControler(){return CFPS_CONTROLER::GetFPSControler();};
//getTime�}�N��
inline void FPS_GetTime(){ GetFPSControler()->getTime(); };
//checktime�}�N��
inline bool FPS_CheckTime(){ return GetFPSControler()->checkTime(); };
//updatetime�}�N��
inline void FPS_UpdateTime(){ GetFPSControler()->updateTime(); };
//Measurement_FPS�}�N��
inline void FPS_Measurement(){ GetFPSControler()->Measurement_FPS(); };
//FPSDraw�}�N��
inline void FPS_Draw(){ GetFPSControler()->Draw(); }
//-------------------------------------------------------------------------------

#endif