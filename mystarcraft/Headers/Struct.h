#pragma once

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9		pTexture;
	D3DXIMAGE_INFO			ImgInfo;
}TEXINFO;

typedef struct terrainInfo
{
	BYTE			byTerrain_ID; //어느 지형
	BYTE			byGroup_ID; //지형의 어느그룹
	BYTE			byGroup_sequence; //그룹의 몇번째
	BYTE			bysortLV; //소팅레벨

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
	BYTE			byFloor; /*0층이 지하*/
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
	D3DCOLOR			   fog_color[TEAM_END];
	FOGSIGHT_OPTION		   eSight[TEAM_END];
	int					   overlap_cnt[TEAM_END]; 
	foginfo()
	{
		for(int i = 0; i < TEAM_END; ++i)
		{
			fog_color[i] = D3DCOLOR_ARGB(255,255,255,255);
			eSight[i] = FOG_BLACK;
		}
		memset(overlap_cnt , 0 , sizeof(overlap_cnt));
	}
}FOG_INFO;
typedef struct creepinfo
{
	//unsigned short		   creep_bit;
	//BYTE				   creep_bit[4];	
	ULONG				   creep_bit;
	USHORT				   creep_cnt;
	BYTE				   creep_sequence;
	bool				   bcreep_install;

	creepinfo()
	{		
		creep_cnt = 0;
		creep_sequence = 0;
		creep_bit		 = 0x00000000;
		bcreep_install	 = false;

		//memset(creep_bit , 0, sizeof(BYTE)*4);
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
	int					iCost; //거리비용 G + H
	float				G;//현재노드와 시작점의 거리
	float				H;//현재 노드와 도착점의 거리
	float				X; //길 못찾았을때를 대비한 가중치
	int				index;//노드의 인덱스
	tagAstarNode*	pParent;//부모의 노드
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
	DAMAGE_TYPE			eDamageType;     /*진동형 노말형 폭발형*/
	DAMAGE_TYPE			eAirDamageType;     /*진동형 노말형 폭발형*/

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
	MOVE_TYPE			eMoveType;           /*땅인지 비행인지*/
	ATTACK_SEARCH_TYPE	eAttackType;     /*땅만공격 하는지 , 공중만 공격하는건지, 아무거나 다하는지*/

	ARMOR_TYPE			eArmorType;
	STATE				state; // 상태
	ORDER				order; //받은 명령
	UNIT_SIZE			esize;
	OBJ_RACE			erace;
	TRIBE				etribe;

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
	bool				is_hide;
	int					detect[TEAM_END];
	tagunitinfo()
	{
		etribe = TRIBE_NONE;
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
		is_hide = false;
		
		for(int i = 0; i < TEAM_END; ++i)
			detect[i] = 1;
	}
}UNITINFO;

typedef struct tag_preivew
{
	const TCHAR* objname;
	D3DXVECTOR2	 vpos;
	D3DXVECTOR2	 vcenter_pos;
	TERRAN_BUILD_TECH	 ebuild;
	MYRECT<float>	vtx;
	USHORT			 icol;
	USHORT			 irow;
	tag_preivew()
	{
		icol = 0;
		irow = 0;
		ebuild = BUILD_NONE;
		objname = NULL;
		vtx.left = 0;
		vtx.right = 0;
		vtx.top = 0;
		vtx.bottom = 0;
	}
}PREVIEW_INFO;

typedef struct tag_upginfo
{
	bool proceeding; //진행중이냐
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
	OBJID eid;
	const TCHAR* texkey;

	tag_production()
	{
		texkey = L"";
		curtime = 0.f;
		maxtime = 0.f;
		eid = OBJ_NONE;
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
