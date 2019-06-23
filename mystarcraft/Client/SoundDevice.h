#pragma once


// 사운드 관련 소스 확인 및 분석 해야할 곳

// 1. stdafx.h의 헤더파일 추가 부분
// 2. maingame 클래스 Initialize 및 Release 함수
// 3. Stage 클래스 Initialize 및 Progress 함수 
// 4. 프로젝트 폴더(하부 폴더로 Texture 폴더처럼 Sound폴더가 있는 지 확인)
// 5. 더불어서 wav파일만 지원되는 형식임. 
// 6. 혹여 집에서 작업하다가 안된다면 directx sdk의 문제임, 미 설치 시 인터넷에서 
//	  다운받아 필수적으로 설치해야 됨. june2010버전이 있고, 설치 후 끝다는 것이 아님.

#include "Include.h"

const int volumeValue = -2500;

class CSoundDevice
{
	DECLARE_SINGLETON(CSoundDevice);
public:
	typedef struct tagSound
	{
		float				timeGap;
		float				curTime;
		int					duplicatePlayCnt;
		int					curPlaycnt;
		bool				setPlay;

		tagSound()
		{
			timeGap = 0.1f;
			curTime = 0.f;
			duplicatePlayCnt = 0;
			curPlaycnt = 0;
			setPlay = true;
		}
	}SOUND_INFO;

	typedef struct tagBattleSoundPosition
	{
		D3DXVECTOR2			vPos;
		LPDIRECTSOUNDBUFFER	soundBuffer;
		int					idx;

		tagBattleSoundPosition()
		{
			idx = 0;
			soundBuffer = NULL;
			vPos = D3DXVECTOR2(0.f,0.f);
		}
	}BATTLE_SOUND_INFO;
private:
	//directx sdk (마이크로소프트에서 컴퓨터에있는 하드웨어를 쉽게 상요할수 있게
	// 여러가지 함수와, 데이터형을 제공해놓은 헤더파일과 라이브러리파일의 집합.)
	//다이렉트 사운드 인터페이스

	LPDIRECTSOUND8					m_pSound; //사운드카드를 대표하는 객체.
	vector<LPDIRECTSOUNDBUFFER8>	m_vecSoundBuff;	//사운드파일을 로드해서 저장할 벡터

	vector<LPDIRECTSOUNDBUFFER8>	m_vecBgmBuff;
	vector<LPDIRECTSOUNDBUFFER8>	m_vecBattleBuff;	
	vector<LPDIRECTSOUNDBUFFER8>	m_vecVoiceBuff;
	vector<LPDIRECTSOUNDBUFFER8>	m_vecEffectBuff;

	list<BATTLE_SOUND_INFO>			m_copyBattleSnd;
	list<LPDIRECTSOUNDBUFFER>		m_copyBgmSnd;
	list<LPDIRECTSOUNDBUFFER>		m_copyVoiceSnd[OBJ_END];
	list<LPDIRECTSOUNDBUFFER>		m_copyEffectSnd;

	SOUND_INFO						m_battleSndInfo[SND_B_END];
	SOUND_INFO						m_voiceSndInfo[SND_V_END];

	int								m_bgmCnt;
public:
	HRESULT					Init(void);	// 장치를 초기화 하는역활.
	LPDIRECTSOUNDBUFFER8	LoadWave(TCHAR* pFileName);	//파일경로를 통해 파일을 읽어 들이겠다.
	
	void SoundStop(int iIndex);
	bool SoundPlaying(int iIndex);	// 현재 iIndex번째 사운드가 재생되고있는지 아닌지.

	void PlayLoadingSound(const SOUND_BGM iIndex /*몇번째 파일을 재생할래?*/, DWORD dwFlag/*무한루프, 한번*/); //파일을 재생 하겠다.

	bool PlayBattleSound(const SOUND_BATTLE iIndex , const D3DXVECTOR2	vpos);	// 현재 iIndex번째 사운드가 재생되고있는지 아닌지.
	bool PlayBgmSound(const SOUND_BGM iIndex , const DWORD flag);	// 현재 iIndex번째 사운드가 재생되고있는지 아닌지.
	bool PlayEffSound(const SOUND_EFF iIndex , const DWORD flag);
	bool PlayVoiceSound(const SOUND_VOICE iIndex , const OBJID id);

	bool ReadSoundResource(TCHAR* szpath);
	bool ReadLodingSound(void);
	HRESULT ReadSound(TCHAR* render_szpath , const char* path , vector<LPDIRECTSOUNDBUFFER8>& vecbuffer);

	void SetUnitVoice(const OBJID id);
	void SetVolume(int iIndex , LONG volval);
	void StageUpdate(void);

	void ReleaseSound(void);
private:
	CSoundDevice(void);
	~CSoundDevice(void);
};
