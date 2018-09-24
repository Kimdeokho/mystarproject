#pragma once
#include "Zerg_Building.h"

class CCom_Larvahatch;
class CHatchery :
	public CZerg_building
{
private:
	CCom_Larvahatch*		m_com_larvahatch;
public:
	void	rallypoint_pathfinding(void);
public:
	virtual void Initialize(void);
	virtual void Update(void);
	virtual void Render(void);

	virtual void Inputkey_reaction(const int& nkey);
	virtual void Inputkey_reaction(const int& firstkey , const int& secondkey);
	virtual void Input_cmd(const int& nkey , bool* waitkey);
	virtual void Input_cmd(const int& firstkey , const int& secondkey);
	virtual void Update_Cmdbtn(void);
	virtual void Update_Wireframe(void);

	virtual void Dead(void);
	virtual void Release(void);
public:
	
public:
	CHatchery(void);
	CHatchery(const float& fbuildtime);
	virtual ~CHatchery(void);
};
