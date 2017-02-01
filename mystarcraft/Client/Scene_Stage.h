#pragma once
#include "sceneobj.h"

class CScene_Stage :
	public CSceneObj
{
public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	void LoadData(void);
public:
	CScene_Stage(void);
	~CScene_Stage(void);
};
