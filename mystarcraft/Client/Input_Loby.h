#pragma once
#include "input_interface.h"

class CInput_Loby :
	public CInput_Interface
{
public:
	void Initialize(void);
	void Update(void);
	void Reelase(void);
public:
	CInput_Loby(void);
	~CInput_Loby(void);
};
