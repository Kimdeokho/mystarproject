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
	//QueryPerformanceCounter Output 파라미터에는 현재 count가 설정된다.
	//QueryPerformanceFrequency Output 파라미터에는 초당 count 가 설정된다.

	return S_OK;
}

float CTimeMgr::GetTime( void )
{
	return m_fTime;
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
}

