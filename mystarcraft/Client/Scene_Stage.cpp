#include "StdAfx.h"
#include "Scene_Stage.h"

#include "TileManager.h"
#include "ObjMgr.h"
CScene_Stage::CScene_Stage(void)
{
}

CScene_Stage::~CScene_Stage(void)
{
}

HRESULT CScene_Stage::Initialize(void)
{
	CTileManager::GetInstance()->Initialize();

	LoadData();	

	return S_OK;
}

void CScene_Stage::Update(void)
{

}
void CScene_Stage::Render(void)
{
	CTileManager::GetInstance()->RenderTile();
}
void CScene_Stage::Release(void)
{

}

void CScene_Stage::LoadData(void)
{
	HANDLE hFile = CreateFile(L"../Data/map/test1.dat" , 
		GENERIC_READ , 0 , NULL , OPEN_EXISTING , 0 , NULL);

	CTileManager::GetInstance()->LoadTileData(hFile);
	CObjMgr::GetInstance()->LoadObj(hFile);
}
