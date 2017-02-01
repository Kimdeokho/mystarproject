#include "StdAfx.h"
#include "SceneObj.h"

#include "Device.h"
CSceneObj::CSceneObj(void)
{
	m_pDevice = CDevice::GetInstance();
}

CSceneObj::~CSceneObj(void)
{
}
