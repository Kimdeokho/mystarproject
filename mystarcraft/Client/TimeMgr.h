#pragma once

#include "Include.h"

class CTimeMgr
{
private:
	static const float FIXTIME;
public:
	DECLARE_SINGLETON(CTimeMgr)

private:
	LARGE_INTEGER		m_FrameTime;
	LARGE_INTEGER		m_FixTime;
	LARGE_INTEGER		m_LastTime;
	LARGE_INTEGER		m_CpuTick;

	float		m_fTime;
	const int*	m_fps;



	LARGE_INTEGER	sStartTime; 
	float			mDeltaTime;

	double			mLastFrameStartTime;
	float			mFrameStartTimef;
	double			mPerfCountDuration;
public:
	float	GetTime(void);
	double	GetTime2(void);
	const int&	GetFps(void);
	void		SetFps(const int* fps);
public:
	HRESULT InitTime(void);
	void SetTime(void);
private:
	CTimeMgr(void);
public:
	~CTimeMgr(void);
};
