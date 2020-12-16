#include "StdAfx.h"
#include "SoundDevice.h"

#include "TimeMgr.h"
#include "ScrollMgr.h"
#include "RoomSession_Mgr.h"

IMPLEMENT_SINGLETON(CSoundDevice);

CSoundDevice::CSoundDevice(void)
{
}

CSoundDevice::~CSoundDevice(void)
{
	ReleaseSound();

	for (size_t i = 0; i < m_vecSoundBuff.size(); ++i)
	{
		m_vecSoundBuff[i]->Release();
	}
	m_vecSoundBuff.clear();

	m_pSound->Release();

}
HRESULT CSoundDevice::Init(void)
{
	//사운드 디바이스 생성
	if(FAILED(DirectSoundCreate8(NULL, &m_pSound, NULL)))
	{
		MessageBox(g_hWnd, L"사운드디바이스생성실패", L"SYSTEM ERROR", MB_OK);
		return E_FAIL;
	}

	//사운드 디바이스 협조레벨 설정.
	if(FAILED(m_pSound->SetCooperativeLevel(g_hWnd, DSSCL_NORMAL)))
	{
		MessageBox(g_hWnd, L"사운드디바이스 협조레벨 설정", L"SYSTEM ERROR", MB_OK);
		return E_FAIL;
	}

	m_bgmCnt = 0;
	m_battleSndCnt = 0;

	m_setplay = true;
	m_curtime = 0.f;
	return S_OK;
}

//사운드파일을 읽기 위한함수(경로)
LPDIRECTSOUNDBUFFER8 CSoundDevice::LoadWave(TCHAR* pFileName)
{
	HMMIO	hFile;

	//CreateFile
	hFile = mmioOpen(pFileName, NULL, MMIO_READ);//wave파일을 연다.

	//정크구조체.
	MMCKINFO	pParent;
	memset(&pParent, 0, sizeof(pParent));
	pParent.fccType = mmioFOURCC('W','A','V','E');
	mmioDescend(hFile, &pParent, NULL, MMIO_FINDRIFF);

	MMCKINFO	pChild;
	memset(&pChild, 0, sizeof(pChild));
	pChild.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend(hFile, &pChild, &pParent, MMIO_FINDCHUNK);

	WAVEFORMATEX	wft;
	memset(&wft, 0, sizeof(wft));
	mmioRead(hFile, (char*)&wft, sizeof(wft));

	mmioAscend(hFile, &pChild, 0);
	pChild.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(hFile, &pChild, &pParent, MMIO_FINDCHUNK);

	DSBUFFERDESC	BuffInfo;
	memset(&BuffInfo, 0, sizeof(DSBUFFERDESC));
	BuffInfo.dwBufferBytes = pChild.cksize;
	BuffInfo.dwSize = sizeof(DSBUFFERDESC);

	BuffInfo.dwFlags = DSBCAPS_STATIC;
	BuffInfo.dwFlags |=  DSBCAPS_CTRLVOLUME; //소리조절
	BuffInfo.dwFlags |=  DSBCAPS_LOCSOFTWARE; 

	BuffInfo.lpwfxFormat = &wft;

	LPDIRECTSOUNDBUFFER		SoundBuff = NULL;
	LPDIRECTSOUNDBUFFER8	SoundBuff8;


	HRESULT hr = m_pSound->CreateSoundBuffer(&BuffInfo, &SoundBuff, NULL); 
	if (SUCCEEDED(hr)) 
	{ 
		hr = SoundBuff->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)&SoundBuff8);
		SoundBuff->Release();
	} 
	else
		return NULL;


	void *pWrite1 = NULL, *pWrite2 = NULL;
	DWORD dwlength1, dwlength2;

	SoundBuff8->Lock(0, pChild.cksize, &pWrite1, &dwlength1
		, &pWrite2, &dwlength2, 0);

	if(pWrite1 > 0)
		mmioRead(hFile, (char*)pWrite1, dwlength1);
	if(pWrite2 > 0)
		mmioRead(hFile, (char*)pWrite2, dwlength2);

	SoundBuff8->Unlock(pWrite1, dwlength1, pWrite2, dwlength2);	

	mmioClose(hFile, 0);

	return SoundBuff8;
}

void CSoundDevice::PlayLoadingSound(const SOUND_BGM iIndex, DWORD dwFlag)
{
	if(iIndex < 0 || iIndex > (signed)m_vecSoundBuff.size())
		return;

	m_vecSoundBuff[iIndex]->SetCurrentPosition(0);

	DWORD			dwFre;

	m_vecSoundBuff[iIndex]->GetFrequency(&dwFre);

	/////중복 재생
	LPDIRECTSOUNDBUFFER temp;
	m_pSound->DuplicateSoundBuffer(m_vecSoundBuff[iIndex] , &temp);

	temp->Play(0,0,dwFlag);
	temp->SetVolume( defaultVolumeValue );

	m_copyBgmSnd.push_back(temp);
	/////중복 재생

	//입력값 * 100 - 10000 = -10000~0 -> 0~100
}
bool CSoundDevice::PlayBgmSound(const SOUND_BGM iIndex , const DWORD flag)
{
	LPDIRECTSOUNDBUFFER temp;
	m_pSound->DuplicateSoundBuffer(m_vecBgmBuff[iIndex] , &temp);

	temp->SetVolume( defaultVolumeValue );
	temp->Play(0,0,flag);

	m_copyBgmSnd.push_back(temp);

	return true;
}
bool CSoundDevice::PlayBattleSound(const SOUND_BATTLE iIndex, const D3DXVECTOR2	vpos , const int volval)
{
	if(!CScrollMgr::inside_camera(vpos.x , vpos.y))
		return false;

	m_battleSndInfo[iIndex].timeGap = 0.23f;
	if(m_battleSndCnt > 4)
		return false;

	if(m_battleSndInfo[iIndex].setPlay && m_battleSndInfo[iIndex].duplicatePlayCnt < 1)
	{
		if(m_battleSndInfo[iIndex].curPlaycnt <= 3)
		{
			m_battleSndInfo[iIndex].setPlay = false;
			m_battleSndInfo[iIndex].duplicatePlayCnt += 1;
			m_battleSndInfo[iIndex].curPlaycnt += 1;

			LPDIRECTSOUNDBUFFER temp;
			BATTLE_SOUND_INFO	info;
			m_pSound->DuplicateSoundBuffer(m_vecBattleBuff[iIndex] , &temp);

			temp->SetVolume( volval );
			temp->Play(0,0,0);

			info.soundBuffer = temp;
			info.vPos = vpos;
			info.idx = iIndex;
			m_copyBattleSnd.push_back(info);
		}
	}

	return true;
}

bool CSoundDevice::PlayEffSound(const SOUND_EFF iIndex , const DWORD flag)
{
	if(!m_copyEffectSnd[iIndex].empty())
		return false;

	LPDIRECTSOUNDBUFFER temp;
	m_pSound->DuplicateSoundBuffer(m_vecEffectBuff[iIndex] , &temp);

	temp->SetVolume( defaultVolumeValue );
	temp->Play(0,0,0);

	m_copyEffectSnd[iIndex].push_back(temp);

	return true;
}

bool CSoundDevice::PlayVoiceSound(const SOUND_VOICE iIndex , const OBJID id )
{
	if(m_vecVoiceBuff.empty())
		return false;


	unsigned int idx = iIndex;
	if(m_copyVoiceSnd[id].empty() &&  idx < m_vecVoiceBuff.size())
	{
		LPDIRECTSOUNDBUFFER temp;
		m_pSound->DuplicateSoundBuffer(m_vecVoiceBuff[idx] , &temp);

		temp->SetVolume( defaultVolumeValue );
		temp->Play(0,0,0);

		m_copyVoiceSnd[id].push_back(temp);
	}


	return true;
}
void CSoundDevice::StageUpdate(void)
{
	if(!m_copyBgmSnd.empty())
	{
		list<LPDIRECTSOUNDBUFFER>::iterator iter = m_copyBgmSnd.begin();
		list<LPDIRECTSOUNDBUFFER>::iterator iter_end = m_copyBgmSnd.end();

		DWORD dwStatus = 0;

		for( ; iter != iter_end; )
		{						
			(*iter)->GetStatus(&dwStatus);

			if( dwStatus & DSBSTATUS_PLAYING )
				++iter;
			else
			{
				//종족 구분후 다음곡으로 넘김
				(*iter)->Release();
				iter = m_copyBgmSnd.erase(iter);

				++m_bgmCnt;
				m_bgmCnt %= 3;

				if(!wcscmp(L"Terran" ,CRoomSession_Mgr::GetInstance()->GetMyinfo()->TRIBE))
					PlayBgmSound( SOUND_BGM(SND_BGM_T1 + m_bgmCnt) , 0);
				else if(!wcscmp(L"Zerg" ,CRoomSession_Mgr::GetInstance()->GetMyinfo()->TRIBE))
					PlayBgmSound( SOUND_BGM(SND_BGM_Z1 + m_bgmCnt) , 0);
			}
		}
	}

	if(!m_copyBattleSnd.empty())
	{
		list<BATTLE_SOUND_INFO>::iterator iter = m_copyBattleSnd.begin();
		list<BATTLE_SOUND_INFO>::iterator iter_end = m_copyBattleSnd.end();

		DWORD dwStatus = 0;

		for( ; iter != iter_end; )
		{			
			(*iter).soundBuffer->GetStatus(&dwStatus);
			
			if( dwStatus & DSBSTATUS_PLAYING )
				++iter;
			else
			{
				m_battleSndInfo[(*iter).idx].curPlaycnt -= 1;
				//m_battleSndInfo.curPlaycnt -= 1;
				(*iter).soundBuffer->Release();
				iter = m_copyBattleSnd.erase(iter);
			}
		}
	}

	for(int i = 0; i < SND_B_END; ++i)
	{		
		m_battleSndCnt = 0;
		m_battleSndInfo[i].duplicatePlayCnt = 0;
		if( m_battleSndInfo[i].curTime > m_battleSndInfo[i].timeGap)
		{
			m_battleSndInfo[i].curTime = 0.f;
			m_battleSndInfo[i].setPlay = true;
		}

		if(m_battleSndInfo[i].curPlaycnt > 0)
		{
			m_battleSndCnt += 1;
			m_battleSndInfo[i].curTime += GETTIME;
		}
		else
		{
			m_battleSndInfo[i].curTime = 0;
			m_battleSndInfo[i].setPlay = true;
		}
	}


	for(int i = 0; i < OBJ_END; ++i)
	{
		if(!m_copyVoiceSnd[i].empty())
		{
			list<LPDIRECTSOUNDBUFFER>::iterator iter = m_copyVoiceSnd[i].begin();
			list<LPDIRECTSOUNDBUFFER>::iterator iter_end = m_copyVoiceSnd[i].end();

			DWORD dwStatus = 0;

			for( ; iter != iter_end; )
			{							
				(*iter)->GetStatus(&dwStatus);

				if( dwStatus & DSBSTATUS_PLAYING )
					++iter;
				else
				{
					(*iter)->Release();
					iter = m_copyVoiceSnd[i].erase(iter);
				}
			}
		}
	}

	for(int i = 0; i < SND_EFF_END; ++i)
	{
		if(!m_copyEffectSnd[i].empty())
		{
			list<LPDIRECTSOUNDBUFFER>::iterator iter = m_copyEffectSnd[i].begin();
			list<LPDIRECTSOUNDBUFFER>::iterator iter_end = m_copyEffectSnd[i].end();

			DWORD dwStatus = 0;

			for( ; iter != iter_end; )
			{							
				(*iter)->GetStatus(&dwStatus);

				if( dwStatus & DSBSTATUS_PLAYING )
					++iter;
				else
				{
					(*iter)->Release();
					iter = m_copyEffectSnd[i].erase(iter);
				}
			}
		}
	}

}
bool CSoundDevice::ReadLodingSound(void)
{
	TCHAR temp[256] = {0};

	ReadSound(temp , "../Data/sound/bgm_soundpath.txt" , m_vecSoundBuff);
	return true;
}
bool CSoundDevice::ReadSoundResource(TCHAR* render_szpath)
{
	ReadSound(render_szpath, "../Data/sound/bgm_soundpath.txt" , m_vecBgmBuff);
	ReadSound(render_szpath, "../Data/sound/battle_soundpath.txt" , m_vecBattleBuff);
	ReadSound(render_szpath, "../Data/sound/effect_soundpath.txt" , m_vecEffectBuff);
	ReadSound(render_szpath, "../Data/sound/voice_soundpath.txt" , m_vecVoiceBuff);

	return true;
}

HRESULT CSoundDevice::ReadSound(TCHAR* render_szpath , const char* path , vector<LPDIRECTSOUNDBUFFER8>& vecbuffer)
{
	wifstream LoadFile;

	LoadFile.open(path , ios::in);

	if(!LoadFile.is_open())
	{
		ERR_MSG(L"Read_SingleImagePath ERROR");
		return E_FAIL;
	}

	LPDIRECTSOUNDBUFFER8 temp;
	TCHAR	szPath[MAX_PATH] = L"";

	while(!LoadFile.eof())
	{
		LoadFile.getline(szPath, MAX_PATH);
		lstrcpy(render_szpath , szPath);

		temp = LoadWave(szPath);
		vecbuffer.push_back(temp);
	}

	LoadFile.close();

	return S_OK;
}

void CSoundDevice::SoundStop(int iIndex)
{
	if(iIndex < 0 || iIndex > (signed)m_vecSoundBuff.size())
		return;

	m_vecSoundBuff[iIndex]->Stop();
}

bool CSoundDevice::SoundPlaying(int iIndex)
{
	DWORD	dwStatus = 0;
	m_vecSoundBuff[iIndex]->GetStatus(&dwStatus);

	if(dwStatus & DSBSTATUS_PLAYING)
		return true;
	return false;	
}

void CSoundDevice::SetVolume(int iIndex , LONG volval)
{
	m_vecSoundBuff[iIndex]->SetVolume(volval);
}

void CSoundDevice::ReleaseSound(void)
{
	if(!m_copyBgmSnd.empty())
	{
		list<LPDIRECTSOUNDBUFFER>::iterator iter = m_copyBgmSnd.begin();
		list<LPDIRECTSOUNDBUFFER>::iterator iter_end = m_copyBgmSnd.end();

		for( ; iter != iter_end; ++iter)
			(*iter)->Release();

		m_copyBgmSnd.clear();
	}

	for(int i = 0; i < OBJ_END; ++i)
	{
		if(!m_copyVoiceSnd[i].empty())
		{
			list<LPDIRECTSOUNDBUFFER>::iterator iter = m_copyVoiceSnd[i].begin();
			list<LPDIRECTSOUNDBUFFER>::iterator iter_end = m_copyVoiceSnd[i].end();

			for( ; iter != iter_end; ++iter)
				(*iter)->Release();

			m_copyVoiceSnd[i].clear();
		}
	}

	for(int i = 0; i < SND_EFF_END; ++i)
	{
		if(!m_copyEffectSnd[i].empty())
		{
			list<LPDIRECTSOUNDBUFFER>::iterator iter = m_copyEffectSnd[i].begin();
			list<LPDIRECTSOUNDBUFFER>::iterator iter_end = m_copyEffectSnd[i].end();

			for( ; iter != iter_end; ++iter)
				(*iter)->Release();

			m_copyEffectSnd[i].clear();
		}
	}

	if(!m_copyBattleSnd.empty())
	{
		list<BATTLE_SOUND_INFO>::iterator iter = m_copyBattleSnd.begin();
		list<BATTLE_SOUND_INFO>::iterator iter_end = m_copyBattleSnd.end();

		for( ; iter != iter_end; ++iter)
			(*iter).soundBuffer->Release();

		m_copyBattleSnd.clear();
	}
}

void CSoundDevice::SetUnitVoice(const OBJID id)
{
	static int voiceCnt = 0;
	++voiceCnt;
	voiceCnt %= 4;

	if(OBJ_DRONE == id)
		PlayVoiceSound( SOUND_VOICE(SND_V_DRONE_0 + voiceCnt) , id);
	else if(OBJ_ZERGLING == id)
		PlayVoiceSound( SOUND_VOICE(SND_V_ZERGLING_0 + voiceCnt) , id);
	else if(OBJ_OVERLOAD == id)
		PlayVoiceSound( SOUND_VOICE(SND_V_OVER_0 + voiceCnt) , id);
	else if(OBJ_HYDRA == id)
		PlayVoiceSound( SOUND_VOICE(SND_V_HYDRA_0 + voiceCnt) , id);
	else if(OBJ_MUTAL == id)
		PlayVoiceSound( SOUND_VOICE(SND_V_MUTAL_0 + voiceCnt) , id);
	else if(OBJ_SCOURGE == id)
		PlayVoiceSound( SOUND_VOICE(SND_V_SCOURGE_0 + voiceCnt) , id);
	else if(OBJ_DEFILER == id)
		PlayVoiceSound( SOUND_VOICE(SND_V_DEFILER_0 + voiceCnt) , id);
	else if(OBJ_ULTRA == id)
		PlayVoiceSound( SOUND_VOICE(SND_V_ULTRA_0 + voiceCnt) , id);
	else if(OBJ_LURKER == id)
		PlayVoiceSound( SOUND_VOICE(SND_V_LURKER_0 + voiceCnt) , id);
	else if(OBJ_SCV == id)
		PlayVoiceSound( SOUND_VOICE(SND_V_SCV_0 + voiceCnt) , id);
	else if(OBJ_MARINE == id)
		PlayVoiceSound( SOUND_VOICE(SND_V_MARINE_0 + voiceCnt) , id);
	else if(OBJ_FIREBAT == id)
		PlayVoiceSound( SOUND_VOICE(SND_V_FB_0 + voiceCnt) , id);
	else if(OBJ_GHOST == id)
		PlayVoiceSound( SOUND_VOICE(SND_V_GST_0 + voiceCnt) , id);
	else if(OBJ_MEDIC == id)
		PlayVoiceSound( SOUND_VOICE(SND_V_MD_0 + voiceCnt) , id);
	else if(OBJ_VULTURE == id)
		PlayVoiceSound( SOUND_VOICE(SND_V_VUL_0 + voiceCnt) , id);
	else if(OBJ_TANK == id)
		PlayVoiceSound( SOUND_VOICE(SND_V_TANK_0 + voiceCnt) , id);
	else if(OBJ_GOLIATH == id)
		PlayVoiceSound( SOUND_VOICE(SND_V_GOL_0 + voiceCnt) , id);
	else if(OBJ_WRAITH == id)
		PlayVoiceSound( SOUND_VOICE(SND_V_WRAITH_0 + voiceCnt) , id);
	else if(OBJ_DROPSHIP == id)
		PlayVoiceSound( SOUND_VOICE(SND_V_DROPSHIP_0 + voiceCnt) , id);
	else if(OBJ_BATTLE == id)
		PlayVoiceSound( SOUND_VOICE(SND_V_BATTLE_0 + voiceCnt) , id);
	else if(OBJ_VESSEL == id)
		PlayVoiceSound( SOUND_VOICE(SND_V_VESSLE_0 + voiceCnt) , id);
	else if(OBJ_COMSET == id)
		PlayVoiceSound( SOUND_VOICE(SND_V_TBC ) , id);
}

const int CSoundDevice::defaultVolumeValue = -3000;