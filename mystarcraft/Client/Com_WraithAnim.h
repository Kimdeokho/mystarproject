#pragma once
#include "com_animation.h"

class CCom_WraithAnim :
	public CCom_Animation
{
private:
	bool	m_isclocikng;
	float	m_discolor_time;
public:
	void Clocking_on(void);
	void Clocking_off(void);
public:
	virtual void Initialize(CObj* pobj);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void SetAnimation(const TCHAR* statekey);
public:
	CCom_WraithAnim(D3DXMATRIX& objmat);
	~CCom_WraithAnim(void);
};
