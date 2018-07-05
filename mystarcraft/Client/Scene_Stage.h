#pragma once
#include "sceneobj.h"

class CInput_Stage;
class CScene_Stage :
	public CSceneObj
{
private:
	float	m_fTime;
	TCHAR				m_szFPS[MIN_STR];
	int					m_iFPS;
	CInput_Stage*		m_input;
public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	void LoadData(void);
	void RenderFPS(void);
public:
	CScene_Stage(void);
	~CScene_Stage(void);
};
