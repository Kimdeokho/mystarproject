#include "StdAfx.h"

#include "Loding.h"
#include "TextureMgr.h"
#include "FontMgr.h"
#include "SoundDevice.h"

CLoding::CLoding(void)
{
}

CLoding::CLoding(LODINGFLAG eFlag)
{	
	m_eLodingFlag = eFlag;
}
CLoding::~CLoding(void)
{
	Release();
}

unsigned int APIENTRY LodingStart(void* parg)
{
	CLoding*	pLoding = (CLoding*)parg;

	EnterCriticalSection(&pLoding->GetKey());
	switch(pLoding->GetLodingFlag())
	{
	case CLoding::LODING_BASIC:
		{
			pLoding->Load_BaiscTexture();
			break;
		}		
	}

	LeaveCriticalSection(&pLoding->GetKey());
	_endthreadex(0);
	return 0;
}

void CLoding::Initialize(void)
{
	lstrcpy(m_szPath , L"");

	m_bLodingComplete = false;
	m_pTextureMgr = CTextureMgr::GetInstance();
	m_pSoundDevice = CSoundDevice::GetInstance();

	InitializeCriticalSection(&m_CriticalSection);	

	m_Thread = (HANDLE)_beginthreadex(NULL , 0 , LodingStart ,this , 0 , NULL);

}

CLoding::LODINGFLAG CLoding::GetLodingFlag(void)
{
	return m_eLodingFlag;
}

CRITICAL_SECTION CLoding::GetKey(void)
{
	return m_CriticalSection;
}

void CLoding::Load_BaiscTexture(void)
{
	if(m_pTextureMgr->Read_Texture(m_szPath))
	{
		if(m_pSoundDevice->ReadSoundResource(m_szPath))
			m_bLodingComplete = true;
	}
	
}

void CLoding::Logo_LodingRender(void)
{
	//if(true == m_bLodingRender)
	//CFontMgr::GetInstance()->FontRender(m_szPath , 400.f,500.f , D3DCOLOR_ARGB(255,255,255,255));

	CFontMgr::GetInstance()->Setbatch_Font(m_szPath , (float)BACKBUFFER_SIZEX / 2.f, (float)BACKBUFFER_SIZEY / 1.2f ,D3DCOLOR_ARGB(255,255,255,255));
	CFontMgr::GetInstance()->FontRender();
}

bool CLoding::GetLoadingComplete(void)
{
	return m_bLodingComplete;
}

void CLoding::Release(void)
{
	WaitForSingleObject(m_Thread , INFINITE);
	DeleteCriticalSection(&m_CriticalSection);
	CloseHandle(m_Thread);
}