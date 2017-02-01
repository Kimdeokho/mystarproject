#pragma once
#include "sceneobj.h"

class CLoding;
class CScene_Logo :
	public CSceneObj
{
private:
	CLoding*			m_pLoding;
	LPDIRECT3DTEXTURE9	m_pBackTexture;
	D3DXMATRIX			m_matWorld;
public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CScene_Logo(void);
	~CScene_Logo(void);
};
