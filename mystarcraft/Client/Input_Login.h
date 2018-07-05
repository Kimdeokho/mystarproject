#pragma once
#include "input_interface.h"


class CInput_Login :
	public CInput_Interface
{
public:
	void Initialize(void);
	void Update(void);
	void Reelase(void);
public:
	CInput_Login(void);
	~CInput_Login(void);
};
