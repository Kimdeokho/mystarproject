#pragma once

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9		pTexture;
	D3DXIMAGE_INFO			ImgInfo;
}TEXINFO;

typedef struct terrainInfo
{
	BYTE			byTerrain_ID; //��� ����
	BYTE			byGroup_ID; //������ ����׷�
	BYTE			byGroup_sequence; //�׷��� ���°
	BYTE			bysortLV; //���÷���

	bool operator< (const terrainInfo& temp) const
	{
		return this->bysortLV < temp.bysortLV;
	}
	terrainInfo()
	{
		byTerrain_ID = TERRAIN_HIGHDIRT;
		byGroup_ID = GROUP_FLAT;
		byGroup_sequence = 0;
		bysortLV = 0;
	}
	~terrainInfo()
	{
	}
}TERRAIN_INFO;

typedef struct tagTile
{
	D3DXVECTOR2		vPos;
	BYTE			byOption;
	BYTE			byFloor; /*0���� ����*/
	//list<TERRAIN_INFO*>	terrainList;
	tagTile()
	{
		vPos = D3DXVECTOR2(0,0);
		byOption = 0;
		byFloor = 1;
	}
	~tagTile(){}

}TILE;
typedef struct foginfo
{
	unsigned short		   fog_sequence;
	D3DCOLOR			   fog_color;
	bool				   bLight;
	bool				   bsearch; // ������ �ɵ�
	int					   overlap_cnt;
	int					   obj_id;  // ������ �ɵ�	 
	FOGSIGHT_OPTION		   eSight;
	foginfo()
	{
		obj_id				 = -1;
		overlap_cnt			 = 0;
		fog_sequence		 = 1;
		fog_color = D3DCOLOR_ARGB(255,255,255,255);
		bLight = false;
		bsearch = false;
		eSight = FOG_BLACK;
	}
}FOG_INFO;
typedef struct creepinfo
{
	unsigned short		   creep_bit;
	short				   creep_sequence;
	bool				   bcreep_install;

	creepinfo()
	{		
		creep_sequence = 0;
		creep_bit		 = 0x0000;
		bcreep_install	 = false;
	}
}CREEP_INFO;

template<typename T>
struct MYRECT
{
	T left;
	T right;
	T top;
	T bottom;

	MYRECT(){}
	MYRECT(T _left , T _right, T _top , T _bottom)
	{
		left = _left;
		right = _right;
		top = _top;
		bottom = _bottom;
	}
};

typedef struct fontinfo
{
	float fnotice_time;
	float fX;
	float fY;
	D3DCOLOR font_color;
	TCHAR font[260];
	bool  left_align;
	fontinfo()
	{
		fnotice_time = 0.f;
		fX = 0.f;
		fY = 0.f;
		font_color = D3DCOLOR_ARGB(255,0,255,0);
		left_align = false;
	}
}FONT_INFO;

typedef struct tagInfo
{
	D3DXVECTOR3		vPos;
	D3DXVECTOR3		vDir;
	D3DXVECTOR3		vLook;
	D3DXMATRIX		matWorld;
}INFO;

typedef struct tagImgPath
{
	wstring		wstrObjKey;
	wstring		wstrStateKey;
	wstring		wstrPath;
	int			iCount;
}IMGPATH;

typedef struct tagAstarNode
{
	int					iCost; //�Ÿ���� G + H
	float				G;//������� �������� �Ÿ�
	float				H;//���� ���� �������� �Ÿ�
	float				X; //�� ��ã�������� ����� ����ġ
	int				index;//����� �ε���
	tagAstarNode*	pParent;//�θ��� ���
	D3DXVECTOR2		vPos;

	tagAstarNode()
	{
		G = 0;
		H = 0;
		X = 0;
		iCost = 0;
		index   = 0;
		pParent = NULL;
	}
}PATH_NODE;

typedef struct tagFlowFieldNode
{
	int				iCost;
	int				index;
	short			idestidx;
	bool			bcheck;
	bool			bmove;
	tagFlowFieldNode()
	{
		index = 0;
		iCost = 0;
		idestidx = 0;
		bcheck = false;
		bmove =  true;
	}
}FLOW_NODE;

typedef struct tagFrame
{
	float				fcurframe;
	float				fframespeed;
	int					umax;

	tagFrame(void) 
		: fcurframe(0.f), fframespeed(0.f), umax(0)
	{

	}

	tagFrame(float _frame, float _cnt, int _max)
		: fcurframe(_frame), fframespeed(_cnt), umax(_max) 
	{

	}

}FRAME;
typedef struct myline
{
	float fx1;
	float fy1;

	float fx2;
	float fy2;

	void setpoint(const float& _fx1 , const float& _fy1 , const float& _fx2 , const float& _fy2)
	{
		fx1 = _fx1;
		fy1 = _fy1;
		fx2 = _fx2;
		fy2 = _fy2;
	}
}MYLINE;

typedef struct tagweaponinfo
{
	DAMAGE_TYPE			eDamageType;     /*������ �븻�� ������*/
	DAMAGE_TYPE			eAirDamageType;     /*������ �븻�� ������*/

	int					damage;
	int					airdamage;

	tagweaponinfo()
	{
		eDamageType = DAMAGE_NOMAL;
		eAirDamageType = DAMAGE_NOMAL;
		damage = 0;
		airdamage = 0;
	}

}WEAPON_INFO;

typedef struct tagunitinfo
{
	MOVE_TYPE			eMoveType;           /*������ ��������*/
	ATTACK_SEARCH_TYPE	eAttackType;     /*�������� �ϴ��� , ���߸� �����ϴ°���, �ƹ��ų� ���ϴ���*/

	ARMOR_TYPE			eArmorType;
	STATE				state; // ����
	ORDER				order; //���� ���
	UNIT_SIZE			esize;
	OBJ_RACE			erace;

	float				fbuildtime;
	float				fspeed;

	int					maxhp;
	int					maxmp;
	int					hp;
	int					mp;
	int					armor;

	int					attack_range;
	int					air_attack_range;
	int					search_range;
	int					fog_range;

	bool				is_active;
	bool				is_wait; //����
	tagunitinfo()
	{
		esize = SIZE_NONE;
		erace = OBJRACE_NONE;
		eMoveType = MOVE_NOT;
		eAttackType = ATTACK_ANYTHING;
		eArmorType = ARMOR_MEDIUM;
		state = IDLE;
		order = ORDER_NONE;
		fspeed = 0.f;
		fbuildtime = 0.f;
		hp = 0;
		maxhp = 0;
		maxmp = 0;
		mp = 0;
		armor = 0;
		search_range = 0;
		attack_range = 0;
		air_attack_range = 0;
		fog_range = 64;
		is_active = true;
		is_wait = false;
	}
}UNITINFO;

typedef struct tag_preivew
{
	const TCHAR* objname;
	D3DXVECTOR2	 vpos;
	D3DXVECTOR2	 vcenter_pos;
	TERRAN_BUILD_TECH	 ebuild;
	MYRECT<float>	vtx;
	int			 icol;
	int			 irow;
	tag_preivew()
	{
		icol = 0;
		irow = 0;
		ebuild = BUILD_NONE;
		objname = NULL;
	}
}PREVIEW_INFO;

typedef struct tag_upginfo
{
	bool proceeding; //�������̳�
	int	 upg_cnt;
	int	 obj_num;
	float	curtime;
	float	maxtime;

	tag_upginfo()
	{
		proceeding = false;
		upg_cnt = 0;
		obj_num = 0;
		curtime = 0.f;
		maxtime = 15.f;
	}
}UPG_INFO;

typedef struct tag_production
{
	float curtime;
	float maxtime;
	PRODUCTION_ID eid;
	const TCHAR* texkey;

	tag_production()
	{
		texkey = L"";
		curtime = 0.f;
		maxtime = 0.f;
		eid = PRODUCTION_NONE;
	}
}PRODUCTION_INFO;

typedef struct tag_boarding
{
	void* obj_address;
	int irow;
	int icol;
	const TCHAR* texkey;

	tag_boarding()
	{
		obj_address = NULL;
		irow = 1;
		icol = 1;
		texkey = L"";
	}
}BOARDING_INFO;


typedef struct tagsession_info
{
	WCHAR			USER_ID[32];
	DWORD_PTR		SESSION_ID;
	WCHAR			VIRTUAL_ADDRESS[32];
	USHORT			VIRTUAL_PORT;
	WCHAR			REAL_ADDRESS[32];
	USHORT			REAL_PORT;
}SESSION_INFO;

typedef struct user_info
{
	WCHAR			USER_ID[32];
	DWORD_PTR		SESSION_ID;
	WCHAR			VIRTUAL_ADDRESS[32];
	USHORT			VIRTUAL_PORT;
	WCHAR			REAL_ADDRESS[32];
	USHORT			REAL_PORT;
	WCHAR			TRIBE[32];
	INT				TEAMNUM;
}USER_INFO;
