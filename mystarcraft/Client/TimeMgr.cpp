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


	LARGE_INTEGER perfFreq;
	QueryPerformanceFrequency( &perfFreq );
	mPerfCountDuration = 1.0 / perfFreq.QuadPart;

	QueryPerformanceCounter( &sStartTime );

	mLastFrameStartTime = GetTime2();

	return S_OK;
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
	double currentTime = GetTime2();

	mDeltaTime = ( float ) ( currentTime - mLastFrameStartTime );


	//sleep함수로 프레임을 제어하면 가끔가다 락스텝동기화가 깨진다.

	//frame lock at 30fps
	while( mDeltaTime < FIXTIME ) 
	{
		currentTime = GetTime2();

		mDeltaTime = (float)( currentTime - mLastFrameStartTime );
	}

	mDeltaTime = FIXTIME;

	mLastFrameStartTime = currentTime;
	mFrameStartTimef = static_cast< float > ( mLastFrameStartTime );

}

float CTimeMgr::GetTime( void )
{
	return FIXTIME;
}
double CTimeMgr::GetTime2(void)
{
	LARGE_INTEGER curTime, timeSinceStart;
	QueryPerformanceCounter( &curTime );

	timeSinceStart.QuadPart = curTime.QuadPart - sStartTime.QuadPart;

	//시작후 몇초가 지났는지 반환한다
	return timeSinceStart.QuadPart * mPerfCountDuration;
}

const float CTimeMgr::FIXTIME = 1/30.f;