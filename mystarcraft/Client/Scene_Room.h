#pragma once
#include "sceneobj.h"

class CScene_Room :
	public CSceneObj
{
private:
	LPDIRECT3DTEXTURE9	m_pBackTexture;
	D3DXMATRIX			m_matWorld;
public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CScene_Room(void);
	~CScene_Room(void);
};
