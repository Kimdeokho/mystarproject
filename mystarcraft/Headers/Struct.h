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
	unsigned short		   fog_sequence;
	D3DCOLOR			   fog_color;
	bool				   bLight;
	bool				   bsearch; // 지워도 될듯
	int					   overlap_cnt;
	int					   obj_id;  // 지워도 될듯	 
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
	fontinfo()
	{
		fnotice_time = 0.f;
		fX = 0.f;
		fY = 0.f;
		font_color = D3DCOLOR_ARGB(255,0,255,0);
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
	float				iCost; //거리비용
	float				G;//현재노드와 시작점의 거리
	float				H;//현재 노드와 도착점의 거리
	int				index;//노드의 인덱스
	tagAstarNode*	pParent;//부모의 노드
	D3DXVECTOR2		vPos;

	tagAstarNode()
	{
		G = 0;
		H = 0;
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

typedef struct tagunitinfo
{
	MOVE_TYPE			eMoveType;           /*땅인지 비행인지*/
	ATTACK_SEARCH_TYPE	eAttackType;     /*땅만공격 하는지 , 공중만 공격하는건지, 아무거나 다하는지*/
	DAMAGE_TYPE			eDamageType;     /*진동형 노말형 폭발형*/
	ARMOR_TYPE			eArmorType;
	STATE				estate; // 상태
	ORDER				eorder; //받은 명령

	float				fbuildtime;
	float				fspeed;
	int					damage;
	int					maxhp;
	int					maxmp;
	int					hp;
	int					mp;
	int					attack_range;
	int					search_range;
	int					fog_range;

	tagunitinfo()
	{
		eMoveType = MOVE_GROUND;
		eAttackType = ATTACK_ANYTHING;
		eDamageType = DAMAGE_NOMAL;
		eArmorType = ARMOR_MEDIUM;
		estate = IDLE;
		eorder = ORDER_NONE;
		fspeed = 0.f;
		fbuildtime = 0.f;
		damage = 0;
		hp = 10000;
		maxhp = 0;
		maxmp = 0;
		mp = 0;
		attack_range = 0;
		search_range = 0;		
		fog_range = 64;
	}
}UNITINFO;

typedef struct tag_preivew
{
	const TCHAR* objname;
	D3DXVECTOR2	 vpos;
	D3DXVECTOR2	 vcenter_pos;
	BUILD_TECH	 ebuild;
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