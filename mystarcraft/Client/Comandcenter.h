#pragma once
#include "terran_building.h"

class CComandcenter :
	public CTerran_building
{
private:
	CComponent*					m_com_production;
	const vector<TEXINFO*>*		m_production_tex;

	int						m_ontime;
	int						m_offtime;
	float						m_curtime;
	bool						m_bactive; //Ä¿¸Çµå ºÒºû ¹ÝÂ¦ÀÌ´Â°Å ·»´õ¿©ºÎ
	bool						m_bswitch;
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);	
	virtual void Release(void);
	virtual void Dead(void);
	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual void Input_cmd(const int& nkey, bool* waitkey);
	virtual void Input_cmd(const int& firstkey , const int& secondkey);

	//virtual void create_unit(PRODUCTION_ID eid);

	virtual void Update_Cmdbtn(void);
	virtual void Update_Wireframe(void);
public:
	CComandcenter(void);
	CComandcenter(const float fbuildtime);
	~CComandcenter(void);
};
