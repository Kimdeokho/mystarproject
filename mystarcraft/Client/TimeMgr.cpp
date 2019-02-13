#include "StdAfx.h"
#include "TimeMgr.h"

IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr(void)
{
}

CTimeMgr::~CTimeMgr(void)
{
}

HRESULT CTimeMgr::InitTime( void )
{
	if(!QueryPerformanceCounter(&m_FrameTime) ||
		!QueryPerformanceCounter(&m_FixTime) ||
		!QueryPerformanceCounter(&m_LastTime) ||
		!QueryPerformanceFrequency(&m_CpuTick))
	{
		ERR_MSG(L"Init Time Fail");
		return E_FAIL;
	}
	//QueryPerformanceCounter Output �Ķ���Ϳ��� ���� count�� �����ȴ�.
	//QueryPerformanceFrequency Output �Ķ���Ϳ��� �ʴ� count �� �����ȴ�.

	return S_OK;
}

float CTimeMgr::GetTime( void )
{
	//return m_fTime; //�̰� ����� ��ȯ�Ѵٸ�?
	return FIXTIME;
}
const int& CTimeMgr::GetFps(void)
{
	return *m_fps;
}
void CTimeMgr::SetFps( const int* fps )
{
	m_fps = fps;
}
void CTimeMgr::SetTime( void )
{
	QueryPerformanceCounter(&m_FrameTime);

	if(m_FrameTime.QuadPart - m_LastTime.QuadPart > m_CpuTick.QuadPart) //1�ʰ� �����ٴ¼Ҹ�
	{
		QueryPerformanceFrequency(&m_CpuTick); //CPU Tick�� �������ش�

		m_LastTime.QuadPart = m_FrameTime.QuadPart;
	}


	m_fTime = float(m_FrameTime.QuadPart - m_FixTime.QuadPart) / m_CpuTick.QuadPart;
	if(m_fTime < FIXTIME )
	{
		Sleep( DWORD( (FIXTIME - m_fTime) *1000 ));
		//Sleep( (FIXTIME - (update~render���� �ɸ��ð�)) *1000);
	}
	QueryPerformanceCounter(&m_FrameTime);
	m_FixTime = m_FrameTime;
	
	//ex 30������ ������ �������� m_ftime�� ������ 30�����ӿ� ���� ms�� �����غ���
}

void CTimeMgr::FPS_fix(void)
{
	//m_fTime = float(m_FrameTime.QuadPart - m_FixTime.QuadPart) / m_CpuTick.QuadPart;
	//m_FixTime = m_FrameTime;
	//if(m_fTime < 0.03333f )
	//{
	//	Sleep( DWORD((0.03333f - m_fTime) *1000));
	//	//Sleep( (0.0167 - (update~render���� �ɸ��ð�)) *1000);
	//}	
}

const float CTimeMgr::FIXTIME = 1/60.f;