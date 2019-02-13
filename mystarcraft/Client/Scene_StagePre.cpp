#include "StdAfx.h"
#include "Scene_StagePre.h"

#include "TileManager.h"
#include "Ingame_UIMgr.h"
#include "RoomSession_Mgr.h"

#include "ObjMgr.h"
#include "Mineral.h"
#include "GasResource.h"
#include "SceneMgr.h"
#include "ScrollMgr.h"
#include "Session_Mgr.h"
CScene_StagePre::CScene_StagePre(void)
{
}

CScene_StagePre::~CScene_StagePre(void)
{
}

HRESULT CScene_StagePre::Initialize(void)
{	
	CTileManager::GetInstance()->Initialize();
	
	LoadData();

	return S_OK;
}

void CScene_StagePre::Update(void)
{

}

void CScene_StagePre::Render(void)
{

}
void CScene_StagePre::LoadData(void)
{
	int basecnt = 0;
	DWORD dwbyte = 0;
	HANDLE hFile = CreateFile(L"../Data/map/pyhon.dat" , 
		GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , 0 , NULL);


	ReadFile(hFile , &basecnt, sizeof(int), &dwbyte, NULL); //임시,
	CloseHandle(hFile);


	hFile = CreateFile(L"../Data/map/pyhon.dat" , 
		GENERIC_READ , FILE_SHARE_READ , NULL , OPEN_EXISTING , 0 , NULL);

	CTileManager::GetInstance()->LoadTileData(hFile);
	//CObjMgr::GetInstance()->LoadObj(hFile);

	D3DXVECTOR2 vPos;
	WCHAR		objname[MIN_STR] = L"";
	CObj*		pObj = NULL;

	vector<D3DXVECTOR2>	base_list;
	while(TRUE)
	{
		ReadFile(hFile , &vPos , sizeof(D3DXVECTOR2) , &dwbyte , NULL);
		ReadFile(hFile , objname , sizeof(WCHAR)*MIN_STR , &dwbyte , NULL);
		if(0 == dwbyte)
			break;

		if(!wcscmp(objname ,L"Startbase"))
			base_list.push_back(vPos);

		if(!wcscmp(objname ,L"Mineral"))
		{
			pObj = new CMineral;
			pObj->SetPos(vPos);
			pObj->Initialize();
			CObjMgr::GetInstance()->AddObject(pObj , OBJ_MINERAL);

		}
		else if(!wcscmp(objname ,L"Gas"))
		{
			pObj = new CGasResource;
			pObj->SetPos(vPos);
			pObj->Initialize();
			CObjMgr::GetInstance()->AddObject(pObj , OBJ_GAS);
		}
	}
	CloseHandle(hFile);

	PLACE_DATA* placeinfo = CRoomSession_Mgr::GetInstance()->GetPlaceData();
	int			myslot = CRoomSession_Mgr::GetInstance()->GetMyRoomslot();
	
	CSession_Mgr::GetInstance()->SetTeamNumber((TEAM_NUMBER)placeinfo[myslot].TEAMNUM);

	if(!lstrcmp(placeinfo[myslot].TRIBE , L"Terran"))
		CIngame_UIMgr::GetInstance()->Initialize(TRIBE_TERRAN);
	else if(!lstrcmp(placeinfo[myslot].TRIBE , L"Zerg"))
		CIngame_UIMgr::GetInstance()->Initialize(TRIBE_ZERG);
	

	int idx = 0;
	for(int i = 0; i < 8; ++i)
	{
		if( 0 == placeinfo[i].SESSION_ID)
			continue;

		idx = placeinfo[i].START_SLOT; // 0 ~ 3

		if(CSession_Mgr::GetInstance()->GetSession_Info().SESSION_ID == placeinfo[i].SESSION_ID)
		{
			CScrollMgr::m_fScrollX = base_list[idx].x - BACKBUFFER_SIZEX/2;
			CScrollMgr::m_fScrollY = base_list[idx].y - BACKBUFFER_SIZEY/2;
			//CSession_Mgr::GetInstance()->SetTeamNumber((TEAM_NUMBER)placeinfo[i].TEAMNUM);
		}
		
		
		if(!lstrcmp(placeinfo[i].TRIBE , L"Terran"))
		{			
			CObjMgr::GetInstance()->Place_Terran(base_list[idx] , (TEAM_NUMBER)placeinfo[i].TEAMNUM); //포지션이다. 팀도 넣어야하고, 일꾼도ㅇㅇ			
		}
		else if(!lstrcmp(placeinfo[i].TRIBE , L"Zerg"))
		{
			CObjMgr::GetInstance()->Place_Zerg(base_list[idx] , (TEAM_NUMBER)placeinfo[i].TEAMNUM); //포지션이다. 팀도 넣어야하고, 일꾼도ㅇㅇ			
		}
		else
			CObjMgr::GetInstance()->Place_Terran(base_list[idx] , (TEAM_NUMBER)placeinfo[i].TEAMNUM); //포지션이다. 팀도 넣어야하고, 일꾼도ㅇㅇ
	}

	base_list.clear();
	vector<D3DXVECTOR2>().swap(base_list);

	WRITE_TCP_PACKET(PT_LOAD_COMPLETE , WriteBuffer,WRITE_PT_LOAD_COMPLETE(WriteBuffer));

}

void CScene_StagePre::Release(void)
{

}

