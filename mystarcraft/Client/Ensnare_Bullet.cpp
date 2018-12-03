#include "StdAfx.h"
#include "Ensnare_Bullet.h"

#include "Com_dirBulletAnim.h"
#include "TimeMgr.h"

#include "ObjMgr.h"
#include "GeneraEff.h"
#include "MyMath.h"

#include "Area_Mgr.h"
#include "ScrollMgr.h"

#include "Skill_Ensnare.h"
#include "Com_CC.h"


CEnsnare_Bullet::CEnsnare_Bullet(const D3DXVECTOR2& vdesetpos)
{
	m_vdest_pos = vdesetpos;
}

CEnsnare_Bullet::~CEnsnare_Bullet(void)
{
	Release();
}
void CEnsnare_Bullet::Initialize(void)
{
	m_sortID = SORT_AIR_EFF;

	m_vcurdir = m_vdest_pos - m_vPos;
	D3DXVec2Normalize(&m_vcurdir , &m_vcurdir);

	m_componentlist.insert(COMPONENT_PAIR::value_type(COM_ANIMATION , new CCom_dirBulletAnim(m_matWorld , L"PARASITE_BULLET")));

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Initialize(this);

	m_ftick_distance = GETTIME*450;
}

void CEnsnare_Bullet::Update(void)
{
	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Update();


	if(CMyMath::pos_distance(m_vPos , m_vdest_pos) < m_ftick_distance*m_ftick_distance)
	{
		m_bdestroy = true;
		Dead();
	}

	m_vPos += m_vcurdir*m_ftick_distance;

	m_matWorld._41 = m_vPos.x - CScrollMgr::m_fScrollX;
	m_matWorld._42 = m_vPos.y - CScrollMgr::m_fScrollY;
}

void CEnsnare_Bullet::Render(void)
{

	COMPONENT_PAIR::iterator iter = m_componentlist.begin();
	COMPONENT_PAIR::iterator iter_end = m_componentlist.end();

	for( ; iter != iter_end; ++iter)
		iter->second->Render();
}


void CEnsnare_Bullet::Dead(void)
{
	CObj* peff = new CGeneraEff(L"ENSNARE" , m_vPos , D3DXVECTOR2(1,1) , SORT_AIR_EFF);

	peff->Initialize();
	CObjMgr::GetInstance()->AddEffect(peff);

	//CSkill_Ensnare pskill = new CSkill_Ensnare;
	//

	list<CObj*>* area64 = CArea_Mgr::GetInstance()->GetArea64();
	float range = 32*4;
	MYRECT<float> ensnare_rc;
	ensnare_rc.left		= m_vPos.x - range / 2;
	ensnare_rc.right	= m_vPos.x + range / 2;
	ensnare_rc.top		= m_vPos.y - range / 2;
	ensnare_rc.bottom	= m_vPos.y + range / 2;

	D3DXVECTOR2 vpos = m_vPos;
	D3DXVECTOR2 vstartpos = vpos;
	D3DXVECTOR2 vendpos = vpos;

	if(vstartpos.x - (range + 64) < 0)
		vstartpos.x = 0;
	else
		vstartpos.x = vstartpos.x - (range + 64);

	if(vstartpos.y - (range + 64) < 0)
		vstartpos.y = 0;
	else
		vstartpos.y = vstartpos.y - (range + 64);

	if(vendpos.x + (range + 64) > 4096)
		vendpos.x = 4096;
	else
		vendpos.x = vendpos.x + (range + 64);

	if(vendpos.y + (range + 64) > 4096)
		vendpos.y = 4096;
	else
		vendpos.y = vendpos.y + (range + 64);

	int idx = CMyMath::Pos_to_index(vstartpos , 64);
	vstartpos = CMyMath::index_to_Pos(idx , 64 , 64);
	vpos = vstartpos;

	int colcnt = int((vendpos.x - vstartpos.x) / 64) + 1;
	int rowcnt = int((vendpos.y - vstartpos.y) / 64) + 1;


	list<CObj*>::iterator iter;
	list<CObj*>::iterator iter_end;

	CSkill_Ensnare* pskill = NULL;
	CObj* pobj = NULL;
	CComponent* pcom = NULL;
	for(int i = 0; i < colcnt; ++i)
	{
		for(int j = 0; j < rowcnt; ++j)
		{
			idx = CMyMath::Pos_to_index(vpos , 64);

			if(!area64[idx].empty())
			{
				iter = area64[idx].begin();
				iter_end = area64[idx].end();

				for( ; iter != iter_end; ++iter)
				{
					if(CATEGORY_UNIT != (*iter)->GetCategory())
						continue;

					if( MyIntersectrect(&ensnare_rc , &(*iter)->GetMyRect() ) )
					{
						pobj = (*iter);						
						pcom = pobj->GetComponent(COM_CC);

						if(NULL != pcom)
						{
							pskill = new CSkill_Ensnare(pobj);
							pskill->Initialize();
							((CCom_CC*)pcom)->AddBuff(pskill);
						}
					}
				}
			}

			if(vpos.x + 64 > 4096)
				break;

			vpos.x += 64;
		}

		if(vpos.y + 64 > 4096)
			break;

		vpos.x = vstartpos.x;
		vpos.y += 64;
	}
}

void CEnsnare_Bullet::Release(void)
{

}