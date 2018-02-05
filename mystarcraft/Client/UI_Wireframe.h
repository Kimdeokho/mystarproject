#pragma once
#include "ui.h"

class CUI_Wireframe :
	public CUI
{
private:
	const TCHAR* m_texkey;
	CMD_BTN		 m_btn_id;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CUI_Wireframe(const TCHAR* texkey, const D3DXVECTOR2& vpos);
	~CUI_Wireframe(void);
};
