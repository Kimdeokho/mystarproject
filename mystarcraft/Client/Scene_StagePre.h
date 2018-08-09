#pragma once
#include "sceneobj.h"

class CScene_StagePre :
	public CSceneObj
{
private:
public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	void LoadData(void);
	void RenderFPS(void);
public:
	CScene_StagePre(void);
	~CScene_StagePre(void);
};
