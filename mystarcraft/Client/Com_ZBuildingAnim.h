#pragma once
#include "com_animation.h"

class CCom_ZBuildingAnim :
	public CCom_Animation
{
private:
	bool		m_sighton;
	bool		m_escape;

	const TCHAR*	m_origin_name;
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void SetAnimation(const TCHAR* statekey);
public:
	void	SetOriginal_texname(const TCHAR* statekey);
public:
	CCom_ZBuildingAnim(const TCHAR* objname , D3DXMATRIX& objmat);
	~CCom_ZBuildingAnim(void);
};
