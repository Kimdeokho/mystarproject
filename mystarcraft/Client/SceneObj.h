#pragma once

#include "Include.h"

class CDevice;
class CSceneObj
{
protected:
	CDevice*	m_pDevice;
public:
	virtual HRESULT Initialize(void)PURE;
	virtual void Update(void)PURE;
	virtual void Render(void)PURE;
	virtual void Release(void)PURE;
public:
	CSceneObj(void);
	virtual ~CSceneObj(void);
};
