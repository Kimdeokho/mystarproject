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
	return m_fTime;
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

	if(m_FrameTime.QuadPart - m_LastTime.QuadPart > m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);

		m_LastTime.QuadPart = m_FrameTime.QuadPart;
	}

	m_fTime = float(m_FrameTime.QuadPart - m_FixTime.QuadPart) / m_CpuTick.QuadPart;

	m_FixTime = m_FrameTime;

	//ex 30������ ������ �������� m_ftime�� ������ 30�����ӿ� ���� ms�� �����غ���

}

