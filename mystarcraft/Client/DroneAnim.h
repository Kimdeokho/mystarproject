#pragma once
#include "animation.h"

class CDroneAnim :
	public CAnimation
{
private:

public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CDroneAnim(void);
	~CDroneAnim(void);
};
