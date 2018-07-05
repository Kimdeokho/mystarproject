#pragma once
#include "input_interface.h"

class CInput_Room :
	public CInput_Interface
{
public:
	void Initialize(void);
	void Update(void);
	void Reelase(void);
public:
	CInput_Room(void);
	~CInput_Room(void);
};
