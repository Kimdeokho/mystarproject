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
};

typedef struct fontinfo
{
	float fX;
	float fY;
	D3DCOLOR font_color;
	TCHAR font[260];
	fontinfo()
	{
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
	int				iCost; //�Ÿ����
	int				G;//������� �������� �Ÿ�
	int				H;//���� ���� �������� �Ÿ�
	int				index;//����� �ε���
	tagAstarNode*	pParent;//�θ��� ���
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
	unsigned int		umax;

	tagFrame(void) 
		: fcurframe(0.f), fframespeed(0.f), umax(0)
	{

	}

	tagFrame(float _frame, float _cnt, unsigned int _max)
		: fcurframe(_frame), fframespeed(_cnt), umax(_max) 
	{

	}

}FRAME;