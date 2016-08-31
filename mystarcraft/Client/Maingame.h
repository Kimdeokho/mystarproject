#pragma once

class CMaingame
{
public:
	HRESULT Initialize(void);
	void Update(void);
	void Render(void);
	void Release(void);
public:
	CMaingame(void);
	~CMaingame(void);
};
