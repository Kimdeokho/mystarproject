#include "StdAfx.h"
#include "SceneMgr.h"
#include "Scene_Logo.h"
#include "Scene_Stage.h"
#include "Scene_Login.h"
#include "Scene_Loby.h"
#include "Scene_Room.h"

IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr(void)
{
	m_eCurScene = SCENE_NONE;

	for(int i = 0; i < SCENE_END; ++i)
		m_pSceneObj[i] = NULL;

	m_pSceneObj[SCENE_LOGO] = new CScene_Logo;
	m_pSceneObj[SCENE_STAGE] = new CScene_Stage;
	m_pSceneObj[SCENE_LOGIN] = new CScene_Login;
	m_pSceneObj[SCENE_LOBY] = new CScene_Loby;
	m_pSceneObj[SCENE_ROOM] = new CScene_Room;
}

CSceneMgr::~CSceneMgr(void)
{
	Release();
}

HRESULT CSceneMgr::SetScene( SCENEID eSceneID )
{
	if(NULL != m_pSceneObj[m_eCurScene])
		m_pSceneObj[m_eCurScene]->Release();

	m_eCurScene = eSceneID;
	//Safe_Delete(m_pSceneObj);

	//switch(eSceneID)
	//{
	//case SCENE_LOGO:
	//	m_pSceneObj = new CLogo;
	//	break;

	//case SCENE_STAGE:
	//	m_pSceneObj = new CStage;
	//	break;
	//}

	m_pSceneObj[m_eCurScene]->Initialize();
	m_pSceneObj[m_eCurScene]->Update(); //한바퀴를 돌리고
	m_pSceneObj[m_eCurScene]->Render();
	return S_OK;
}

void CSceneMgr::Update( void )
{
	m_pSceneObj[m_eCurScene]->Update();
}

void CSceneMgr::Render( void )
{
	m_pSceneObj[m_eCurScene]->Render();
}

void CSceneMgr::Release( void )
{
	for(int i = 0; i < SCENE_END; ++i)
	{
		if( NULL != m_pSceneObj[i])
			Safe_Delete(m_pSceneObj[i]);
	}
}

SCENEID CSceneMgr::GetScene(void)
{
	return m_eCurScene;
}
