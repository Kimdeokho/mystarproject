#pragma once

#include "Include.h"

class CTimeMgr
{
public:
	DECLARE_SINGLETON(CTimeMgr)

private:
	LARGE_INTEGER		m_FrameTime;
	LARGE_INTEGER		m_FixTime;
	LARGE_INTEGER		m_LastTime;
	LARGE_INTEGER		m_CpuTick;

	float		m_fTime;
	const int*	m_fps;
public:
	float GetTime(void);
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
