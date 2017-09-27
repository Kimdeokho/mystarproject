#include "StdAfx.h"
#include "Device.h"

#include "Include.h"

IMPLEMENT_SINGLETON(CDevice)

CDevice::CDevice(void)
: m_p3D(NULL)
, m_pDevice(NULL)
, m_pSprite(NULL)
, m_pFont(NULL)
, m_pLine(NULL)
{

}

CDevice::~CDevice(void)
{
	Release();
}

HRESULT CDevice::InitDevice( void )
{
	m_p3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9	devicecaps;
	ZeroMemory(&devicecaps, sizeof(D3DCAPS9));

	if(FAILED(m_p3D->GetDeviceCaps(D3DADAPTER_DEFAULT
		, D3DDEVTYPE_HAL, &devicecaps)))
	{
		ERR_MSG(L"장치 정보 얻기 실패");
		return E_FAIL;	
	}

	DWORD	vp;

	if(devicecaps.DevCaps &	D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dpp);

	if(FAILED(m_p3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, g_hWnd, vp, &d3dpp, &m_pDevice)))
	{
		ERR_MSG(L"장치 생성 실패");
		return E_FAIL;
	}

	if(FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		ERR_MSG(L"스프라이트 객체 생성 실패");
		return E_FAIL;
	}

	D3DXFONT_DESCW		FontInfo;
	ZeroMemory(&FontInfo, sizeof(D3DXFONT_DESCW));

	FontInfo.Height = 0;
	FontInfo.Width = 0;
	FontInfo.Weight = FW_HEAVY;
	FontInfo.CharSet = HANGUL_CHARSET;
	lstrcpy(FontInfo.FaceName, L"굴림");

	if(FAILED(D3DXCreateFontIndirect(m_pDevice, &FontInfo
		, &m_pFont)))
	{
		ERR_MSG(L"Font Init false");
		return E_FAIL;
	}

	if(FAILED(D3DXCreateLine(m_pDevice, &m_pLine)))
	{
		ERR_MSG(L"Line Init false");
		return E_FAIL;
	}

	return S_OK;
}

void CDevice::Release( void )
{
	m_pSprite->Release();
	m_pFont->Release();
	m_pDevice->Release();
	
	m_p3D->Release();
}

void CDevice::SetParameters( D3DPRESENT_PARAMETERS& d3dpp )
{
	d3dpp.BackBufferWidth = BACKBUFFER_SIZEX;
	d3dpp.BackBufferHeight = BACKBUFFER_SIZEY;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3dpp.MultiSampleQuality = 0;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;

	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.EnableAutoDepthStencil = TRUE;

	d3dpp.hDeviceWindow = g_hWnd;
	d3dpp.Windowed = TRUE;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

LPDIRECT3DDEVICE9 CDevice::GetDevice( void )
{
	return m_pDevice;
}

void CDevice::Render_Begin( void )
{
	//m_pDevice->Clear(0, NULL
	//	, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL
	//	, D3DCOLOR_XRGB(0,0,255)/*0xff0000ff*/, 1.f, 0);

	m_pDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CDevice::Render_End( void )
{
	m_pSprite->End();
	m_pDevice->EndScene();
	//m_pDevice->Present(NULL, NULL, NULL, NULL);
}

LPD3DXSPRITE CDevice::GetSprite( void )
{
	return m_pSprite;
}

LPD3DXFONT CDevice::GetFont(void)
{
	return m_pFont;
}

LPD3DXLINE CDevice::GetLine(void)
{
	return m_pLine;
}
