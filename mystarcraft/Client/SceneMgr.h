#pragma once

#include "Include.h"

class CSceneObj;
class CSceneMgr
{
public:
	DECLARE_SINGLETON(CSceneMgr)

private:
	CSceneObj*	m_pSceneObj[SCENE_END];
	SCENEID		m_eCurScene;
public:
	HRESULT SetScene(SCENEID eSceneID);
	void Update(void);
	void Render(void);
	void Release(void);

private:
	CSceneMgr(void);
public:
	~CSceneMgr(void);
};
