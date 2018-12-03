#include "StdAfx.h"
#include "Plague_Bullet.h"

#include "Com_dirBulletAnim.h"
#include "TimeMgr.h"

#include "ObjMgr.h"
#include "GeneraEff.h"
#include "MyMath.h"

#include "Area_Mgr.h"
#include "ScrollMgr.h"

#include "Skill_Plague.h"
#include "Com_CC.h"

CPlague_Bullet::CPlague_Bullet(const D3DXVECTOR2& vdesetpos)
{
	m_vdest_pos = vdesetpos;
}

CPlague_Bullet::~CPlague_Bullet(void)
{
}

void CPlague_Bullet::Initialize(void)
{
	CObj* peff = new CGeneraEff(L"PLAGUE" , m_vdest_pos , D3DXVECTOR2(1,1) , SORT_AIR_EFF);

	peff->Initialize();
	CObjMgr::GetInstance()->AddEffect(peff);


	list<CObj*>* area64 = CArea_Mgr::GetInstance()->GetArea64();
	float range = 32*4;
	MYRECT<float> ensnare_rc;
	ensnare_rc.left		= m_vdest_pos.x - range / 2;
	ensnare_rc.right	= m_vdest_pos.x + range / 2;
	ensnare_rc.top		= m_vdest_pos.y - range / 2;
	ensnare_rc.bottom	= m_vdest_pos.y + range / 2;

	D3DXVECTOR2 vpos = m_vdest_pos;
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

	CSkill_Plague* pskill = NULL;
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
					if(CATEGORY_RESOURCE == (*iter)->GetCategory())
						continue;

					if( MyIntersectrect(&ensnare_rc , &(*iter)->GetMyRect() ) )
					{
						pobj = (*iter);						
						pcom = pobj->GetComponent(COM_CC);

						if(NULL != pcom)
						{
							pskill = new CSkill_Plague(pobj);
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

	m_bdestroy = true;
}

void CPlague_Bullet::Update(void)
{

}

void CPlague_Bullet::Render(void)
{

}

void CPlague_Bullet::Dead(void)
{

}

void CPlague_Bullet::Release(void)
{

}
