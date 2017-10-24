#include "StdAfx.h"
#include "GasResource.h"


#include "Device.h"
#include "Com_Collision.h"

#include "ScrollMgr.h"
#include "TextureMgr.h"
#include "LineMgr.h"
#include "Area_Mgr.h"
#include "MyMath.h"

#include "UI_Select.h"
#include "ObjMgr.h"
#include "FontMgr.h"
CGasResource::CGasResource(void)
{
}

CGasResource::~CGasResource(void)
{
}

void CGasResource::building_area_Initialize(void)
{
	m_curidx32 = CMyMath::Pos_to_index(m_vPos.x , m_vPos.y , 32);
	m_curidx64 = CMyMath::Pos_to_index(m_vPos.x , m_vPos.y , 64);

	m_oldidx64 = m_curidx64;

	//여기서 m_vpos는 애초에 센터위치이다
	D3DXVECTOR2 vweight = D3DXVECTOR2( 48 , 16); //m_vpos에서 가장 왼쪽위로 옮기기위한 값
	D3DXVECTOR2	vstartpos = m_vPos - vweight;
	D3DXVECTOR2 vpos;

	int idx64;
	for(int i = 0; i < 2; ++i)
	{
		vpos.y = vstartpos.y + 32 * i;
		for(int j = 0; j < 4; ++j)
		{
			vpos.x = vstartpos.x + 32 * j;

			idx64 = CMyMath::Pos_to_index(vpos , 64);
			CArea_Mgr::GetInstance()->SetObj_Area64(idx64 , idx64 , this );
		}
	}

	//memcpy(m_oldvtxidx , m_curvtxidx , sizeof(D3DXVECTOR2)*4);
}
void CGasResource::Initialize(void)
{

	m_gasa_mount = 50000;

	m_vertex.left = 64;
	m_vertex.right = 64;
	m_vertex.top =  32;
	m_vertex.bottom = 32;


	building_area_Initialize();

	m_ecategory = RESOURCE;
	m_eteamnumber = TEAM_NONE;
	m_sortID = SORT_GROUND;
	m_eOBJ_NAME = OBJ_GAS;



	m_vecTex = CTextureMgr::GetInstance()->GetGeneralTexture(L"Gas");

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_COLLISION , new CCom_Collision(m_vPos , m_rect , m_vertex)) ) ;	

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		(iter)->second->Initialize(this);


	m_select_ui = new CUI_Select(L"Select122" , m_vPos , 13);
	m_select_ui->Initialize();
	CObjMgr::GetInstance()->AddSelect_UI(m_select_ui);
}
void CGasResource::Update(void)
{
	//CFontMgr::GetInstance()->Setbatch_Font(L"%d" , m_gasa_mount , m_vPos.x - CScrollMgr::m_fScrollX
	//	,m_vPos.y - CScrollMgr::m_fScrollY);

	m_select_ui->Update();

	if( BOARDING == m_unitinfo.estate)
		return;

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();

}
void CGasResource::Render(void)
{
	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;

	m_pSprite->SetTransform(&m_matWorld);
	m_pSprite->Draw((*m_vecTex)[0]->pTexture , NULL , &D3DXVECTOR3(64,32,0) , NULL , D3DCOLOR_ARGB(255,255,255,255));

	//CLineMgr::GetInstance()->collisionbox_render(m_rect);
}
void CGasResource::Release(void)
{

}

void CGasResource::building_area_release(void)
{
	D3DXVECTOR2 vweight = D3DXVECTOR2( 48 , 16); //m_vpos에서 가장 왼쪽위로 옮기기위한 값
	D3DXVECTOR2	vstartpos = m_vPos - vweight;
	D3DXVECTOR2 vpos;

	int idx64;
	for(int i = 0; i < 2; ++i)
	{
		vpos.y = vstartpos.y + 32 * i;
		for(int j = 0; j < 4; ++j)
		{
			vpos.x = vstartpos.x + 32 * j;

			idx64 = CMyMath::Pos_to_index(vpos , 64);
			CArea_Mgr::GetInstance()->ReleaseObj_Area64(idx64 , this );
		}
	}
}

void CGasResource::setgas_amount(const int& iamount)
{
	m_gasa_mount += iamount;
}
