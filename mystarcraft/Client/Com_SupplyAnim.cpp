#include "StdAfx.h"
#include "Com_SupplyAnim.h"

#include "Obj.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
CCom_SupplyAnim::CCom_SupplyAnim(D3DXMATRIX& objmat , float fbuildtime)
:CCom_Animation(objmat)
{
	m_objname = L"T_SUPPLY";

	m_statkey = L"";

	m_fbuildtime = fbuildtime;
}

CCom_SupplyAnim::~CCom_SupplyAnim(void)
{
}

void CCom_SupplyAnim::Initialize(CObj* pobj)
{
	m_part_tex = NULL;

	m_pobj = pobj;

	m_supply_parttex = CTextureMgr::GetInstance()->GetGeneralTexture(L"SUPPLY_PART");
	SetAnimation(L"BUILD");

	m_partframe.fcurframe = 0;
	m_partframe.umax = (m_supply_parttex->size());
	m_partframe.fframespeed = float(m_partframe.umax);
}

void CCom_SupplyAnim::Update(void)
{
	m_frame.fcurframe += GETTIME*m_frame.fframespeed;
	if(m_frame.fcurframe >= m_frame.umax)
	{
		m_frame.fcurframe = 0.f;

		if(L"BUILD" == m_statkey)
		{
			m_pobj->SetState(IDLE);
		}
	}

	if( (int)(m_frame.fcurframe) <= m_frame.umax)
		m_curtex = (*m_generaltex)[int(m_frame.fcurframe)];





	if( IDLE == m_pobj->GetUnitinfo().state)
	{
		m_partframe.fcurframe += GETTIME*m_partframe.fframespeed;
		if(m_partframe.fcurframe >= m_partframe.umax)
		{
			m_partframe.fcurframe = 0.f;
		}
		m_part_tex = (*m_supply_parttex)[ int(m_partframe.fcurframe) ];
	}
}

void CCom_SupplyAnim::Render(void)
{
	m_objshadow_mat = m_objmat;
	m_objshadow_mat._41 -= 8;
	m_objshadow_mat._42 -= 8;

	m_pSprite->SetTransform(&m_objshadow_mat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0) , NULL , D3DCOLOR_ARGB(100,0,0,0));

	m_pSprite->SetTransform(&m_objmat);
	m_pSprite->Draw(m_curtex->pTexture , NULL , &D3DXVECTOR3(float(m_curtex->ImgInfo.Width/2) , float(m_curtex->ImgInfo.Height/2 ) , 0)
		, NULL , D3DCOLOR_ARGB(255,255,255,255));

	if(NULL != m_part_tex)
	{
		m_pSprite->SetTransform(&m_objmat);
		m_pSprite->Draw(m_part_tex->pTexture , NULL , &D3DXVECTOR3(float(m_part_tex->ImgInfo.Width/2) , float(m_part_tex->ImgInfo.Height/2 ) , 0)
			, NULL , D3DCOLOR_ARGB(255,255,255,255));
	}
}

void CCom_SupplyAnim::Release(void)
{

}

void CCom_SupplyAnim::SetAnimation(const TCHAR* statekey)
{
	if(m_statkey != statekey)/*새로 들어오는 상태가 기존들어오는 상태와 다른상태여야 한다.*/
	{
		m_statkey = statekey;

		m_generaltex = CTextureMgr::GetInstance()->GetStateTexture_vecset( m_objname , m_statkey);

		m_frame.fcurframe = 0;

		m_frame.umax = m_generaltex->size();

		if(L"BUILD" == m_statkey)
			m_frame.fframespeed = 4/m_fbuildtime;
		else
			m_frame.fframespeed = (float)m_frame.umax;
	}
}
