#include "StdAfx.h"
#include "LineMgr.h"

#include "Obj.h"
#include "Device.h"
#include "Area_Mgr.h"
#include "ScrollMgr.h"
#include "MyMath.h"
#include "MouseMgr.h"
#include "Ingame_UIMgr.h"
#include "Debug_Mgr.h"
#include "SoundDevice.h"
#include "UnitMgr.h"

IMPLEMENT_SINGLETON(CLineMgr)
CLineMgr::CLineMgr(void)
{
}

CLineMgr::~CLineMgr(void)
{
}
void CLineMgr::Initialize(void)
{
	m_pLine = CDevice::GetInstance()->GetLine();
	m_bSwitch = false;
}
void CLineMgr::SetRectPoint(const D3DXVECTOR2& vstart , const D3DXVECTOR2& vend)
{
	m_RectLine[0] = vstart;

	m_RectLine[1].x = vend.x;
	m_RectLine[1].y = vstart.y;

	m_RectLine[2] = vend;

	m_RectLine[3].x = vstart.x;
	m_RectLine[3].y = vend.y;

	m_RectLine[4] = vstart;
}
void CLineMgr::RectLineRender(void)
{
	if(true == m_bSwitch)
	{
		CDevice::GetInstance()->Render_End();
		CDevice::GetInstance()->Render_Begin();

		m_pLine->SetWidth(1.0f);
		m_pLine->Draw(m_RectLine , 5 , D3DCOLOR_ARGB(255,0,255,0));

		CDevice::GetInstance()->Render_End();
		CDevice::GetInstance()->Render_Begin();
	}
}
void CLineMgr::collisionbox_render(const MYRECT<float>& rc)
{
	if(!CDebug_Mgr::m_dbglist[CDebug_Mgr::DBG_COLLBOX])
		return;

	D3DXVECTOR2 vpt[5];

	vpt[0].x = rc.left - CScrollMgr::m_fScrollX;
	vpt[0].y = rc.top - CScrollMgr::m_fScrollY;

	vpt[1].x = rc.right - CScrollMgr::m_fScrollX;
	vpt[1].y = rc.top - CScrollMgr::m_fScrollY;

	vpt[2].x = rc.right - CScrollMgr::m_fScrollX;
	vpt[2].y = rc.bottom - CScrollMgr::m_fScrollY;

	vpt[3].x = rc.left - CScrollMgr::m_fScrollX;
	vpt[3].y = rc.bottom - CScrollMgr::m_fScrollY;

	vpt[4] = vpt[0];

	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();

	m_pLine->SetWidth(1.0f);
	m_pLine->Draw(vpt , 5 , D3DCOLOR_ARGB(255,0,255,0));

	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();
}
void CLineMgr::minicambox_render(const MYRECT<float>& rc)
{
	D3DXVECTOR2 vpt[5];

	vpt[0].x = rc.left;
	vpt[0].y = rc.top;

	vpt[1].x = rc.right;
	vpt[1].y = rc.top;

	vpt[2].x = rc.right;
	vpt[2].y = rc.bottom;

	vpt[3].x = rc.left;
	vpt[3].y = rc.bottom;

	vpt[4] = vpt[0];

	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();

	m_pLine->SetWidth(1.0f);
	m_pLine->Draw(vpt , 5 , D3DCOLOR_ARGB(255,255,255,255));

	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();
}
void CLineMgr::Select_unit(void)
{	

	D3DXVECTOR2 vMousept;

	//vMousept = CMouseMgr::GetInstance()->GetAddScrollvMousePt();
	vMousept = CMouseMgr::GetInstance()->GetClick_Pos();
	int idx = CMyMath::Pos_to_index(vMousept.x , vMousept.y , 64);


	if( (int)m_RectLine[0].x == (int)m_RectLine[2].x &&
		(int)m_RectLine[0].y == (int)m_RectLine[2].y) 
	{
		//제자리에 찍었을때
		CArea_Mgr::GetInstance()->SelectCheck(vMousept);
	}
	else
	{
		MYRECT<float>	rc;

		if(m_RectLine[0].x < m_RectLine[2].x)
		{
			rc.left = m_RectLine[0].x + CScrollMgr::m_fScrollX - 1.0f;
			rc.right = m_RectLine[2].x + CScrollMgr::m_fScrollX + 1.0f;
		}
		else
		{
			rc.left = m_RectLine[2].x + CScrollMgr::m_fScrollX - 1.0f;
			rc.right = m_RectLine[0].x + CScrollMgr::m_fScrollX + 1.0f;
		}

		if(m_RectLine[0].y < m_RectLine[2].y)
		{
			rc.top = m_RectLine[0].y + CScrollMgr::m_fScrollY - 1.0f;
			rc.bottom = m_RectLine[2].y + CScrollMgr::m_fScrollY + 1.0f;
		}
		else
		{
			rc.top = m_RectLine[2].y + CScrollMgr::m_fScrollY - 1.0f;
			rc.bottom = m_RectLine[0].y + CScrollMgr::m_fScrollY + 1.0f;
		}

		CArea_Mgr::GetInstance()->DragCheck( rc);
	}

	for(int i = 0; i < 5; ++i)
		m_RectLine[i] = D3DXVECTOR2(0,0);

	if(!CUnitMgr::GetInstance()->Getcur_unitlist()->empty())
	{
		OBJID id = CUnitMgr::GetInstance()->Getcur_unitlist()->front()->GetOBJNAME();
		CSoundDevice::GetInstance()->SetUnitVoice(id);
	}

	//CUnitMgr::GetInstance()->Update_Cmdbtn();
	//DragCheck()
}
void CLineMgr::RenderGrid(const int& tilesize , const int& tilecnt)
{
	const int iwidth = 1000;
	const int iheight = 800;

	int col = iwidth / tilesize + 2;
	int row = iheight / tilesize + 2;
	
	D3DXVECTOR2	vPoint[2];

	int scrollX = (int)CScrollMgr::m_fScrollX;
	int scrollY = (int)CScrollMgr::m_fScrollY;

	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();

	for(int j = 0; j < col; ++j)
	{
		int index = j + scrollX/tilesize;
		vPoint[0] = D3DXVECTOR2( float(index*tilesize) - scrollX, 0 - (float)scrollY);
		vPoint[1] = D3DXVECTOR2( float(index*tilesize) - scrollX, float(tilecnt*tilesize) - scrollY);

		CDevice::GetInstance()->GetLine()->SetWidth(1.0f);
		CDevice::GetInstance()->GetLine()->Draw(vPoint , 2 , D3DCOLOR_ARGB(255,0,0,0));
	}
	for(int j = 0; j < row; ++j)
	{
		int index = j + scrollY/tilesize;
		vPoint[0] = D3DXVECTOR2(0 - (float)scrollX ,float(index*tilesize) - scrollY );
		vPoint[1] = D3DXVECTOR2(float (tilecnt*tilesize) - scrollX, float(index*tilesize) - scrollY);

		CDevice::GetInstance()->GetLine()->SetWidth(1.0f);
		CDevice::GetInstance()->GetLine()->Draw(vPoint , 2 , D3DCOLOR_ARGB(255,0,0,0));
	}

	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();
}
void CLineMgr::PathLineRender(const D3DXVECTOR2& vstart , const D3DXVECTOR2& vend , const float fwidth)
{
	D3DXVECTOR2 vtemp[2];

	vtemp[0].x = vstart.x - CScrollMgr::m_fScrollX;
	vtemp[0].y = vstart.y - CScrollMgr::m_fScrollY;

	vtemp[1].x = vend.x - CScrollMgr::m_fScrollX;
	vtemp[1].y = vend.y - CScrollMgr::m_fScrollY;

	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();

	m_pLine->SetWidth(fwidth);
	m_pLine->Draw(vtemp , 2 , D3DCOLOR_ARGB(255,0,255,0));

	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();
}
void CLineMgr::RallyLineRender(void)
{
	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();

	m_pLine->SetWidth(2.0f);
	m_pLine->Draw(m_Rally_Line , 2 , D3DCOLOR_ARGB(255,0,255,0));

	CDevice::GetInstance()->Render_End();
	CDevice::GetInstance()->Render_Begin();
}
void CLineMgr::SetRallyPoint(const D3DXVECTOR2& vstart , const D3DXVECTOR2& vend)
{
	m_Rally_Line[0] = vstart;
	m_Rally_Line[1] = vend;
}
void CLineMgr::SetRenderSwitch(bool bswitch)
{
	m_bSwitch = bswitch;
}