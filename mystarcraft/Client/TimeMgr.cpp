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
	//return m_fTime; 
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
	//고해상도 타이머의 현재 CPU의 클럭수를 얻는 함수입니다. 함수의 원형은 아래와 같습니다
	QueryPerformanceCounter(&m_FrameTime);

	if(m_FrameTime.QuadPart - m_LastTime.QuadPart > m_CpuTick.QuadPart) // 1초가 지났다는소리
	{
		QueryPerformanceFrequency(&m_CpuTick); //CPU Tick을 갱신해준다

		m_LastTime.QuadPart = m_FrameTime.QuadPart;
	}


	m_fTime = float(m_FrameTime.QuadPart - m_FixTime.QuadPart) / m_CpuTick.QuadPart;


	if(m_fTime < FIXTIME )
	{
		Sleep( DWORD( (FIXTIME - m_fTime) *1000 ) ); //이거 정말 부정확하다..
		//Sleep( (FIXTIME - (update~render까지 걸린시간)) *1000);
	}

	QueryPerformanceCounter(&m_FrameTime);
	m_FixTime = m_FrameTime; //이전 프레임의 시간 저장
	

}
void CTimeMgr::FPS_fix(void)
{
	//m_fTime = float(m_FrameTime.QuadPart - m_FixTime.QuadPart) / m_CpuTick.QuadPart;
	//m_FixTime = m_FrameTime;
	//if(m_fTime < 0.03333f )
	//{
	//	Sleep( DWORD((0.03333f - m_fTime) *1000));
	//	//Sleep( (0.0167 - (update~render까지 걸린시간)) *1000);
	//}	
}

const float CTimeMgr::FIXTIME = 1/30.f;