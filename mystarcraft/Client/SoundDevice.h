#pragma once


// ���� ���� �ҽ� Ȯ�� �� �м� �ؾ��� ��

// 1. stdafx.h�� ������� �߰� �κ�
// 2. maingame Ŭ���� Initialize �� Release �Լ�
// 3. Stage Ŭ���� Initialize �� Progress �Լ� 
// 4. ������Ʈ ����(�Ϻ� ������ Texture ����ó�� Sound������ �ִ� �� Ȯ��)
// 5. ���Ҿ wav���ϸ� �����Ǵ� ������. 
// 6. Ȥ�� ������ �۾��ϴٰ� �ȵȴٸ� directx sdk�� ������, �� ��ġ �� ���ͳݿ��� 
//	  �ٿ�޾� �ʼ������� ��ġ�ؾ� ��. june2010������ �ְ�, ��ġ �� ���ٴ� ���� �ƴ�.

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
	//directx sdk (����ũ�μ���Ʈ���� ��ǻ�Ϳ��ִ� �ϵ��� ���� ����Ҽ� �ְ�
	// �������� �Լ���, ���������� �����س��� ������ϰ� ���̺귯�������� ����.)
	//���̷�Ʈ ���� �������̽�

	LPDIRECTSOUND8					m_pSound; //����ī�带 ��ǥ�ϴ� ��ü.
	vector<LPDIRECTSOUNDBUFFER8>	m_vecSoundBuff;	//���������� �ε��ؼ� ������ ����

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
	HRESULT					Init(void);	// ��ġ�� �ʱ�ȭ �ϴ¿�Ȱ.
	LPDIRECTSOUNDBUFFER8	LoadWave(TCHAR* pFileName);	//���ϰ�θ� ���� ������ �о� ���̰ڴ�.
	
	void SoundStop(int iIndex);
	bool SoundPlaying(int iIndex);	// ���� iIndex��° ���尡 ����ǰ��ִ��� �ƴ���.

	void PlayLoadingSound(const SOUND_BGM iIndex /*���° ������ ����ҷ�?*/, DWORD dwFlag/*���ѷ���, �ѹ�*/); //������ ��� �ϰڴ�.

	bool PlayBattleSound(const SOUND_BATTLE iIndex , const D3DXVECTOR2	vpos);	// ���� iIndex��° ���尡 ����ǰ��ִ��� �ƴ���.
	bool PlayBgmSound(const SOUND_BGM iIndex , const DWORD flag);	// ���� iIndex��° ���尡 ����ǰ��ִ��� �ƴ���.
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
