#pragma once

struct tagtest
{
	int a;
	int b;
	int c;
};
typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9		pTexture;
	D3DXIMAGE_INFO			ImgInfo;
}TEXINFO;

typedef struct tagTile
{
	D3DXMATRIX		matWorld;

	BYTE			byTerrain_ID; //어느 지형
	BYTE			byGroup_ID; //지형의 어느그룹
	BYTE			byGroup_sequence; //그룹의 몇번째

	BYTE			byOption;
	BYTE			byFloor; /*0층이 지하*/

	tagTile()
	{
		D3DXMatrixIdentity(&matWorld);
		byTerrain_ID = TERRAIN_DIRT;
		byGroup_ID = GROUP_FLAT;
		byGroup_sequence = 0;
		byOption = 0;
		byFloor = 1;
	}
	~tagTile(){}

}TILE;

typedef struct tagUnitData
{
	wstring		wstrName;
	int			iHp;
	int			iAtt;
	int			iDef;
	BYTE		byJobId;
	BYTE		byDropItem;
}UNITDATA;

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
	float			fCost; //거리비용
	int				iIndex;//노드의 인덱스
	tagAstarNode*	pParent;//부모의 노드
}NODE;

typedef struct tagFrame
{
	float		fFrame;
	float		fCnt;
	float		fMax;

	tagFrame(void) 
		: fFrame(0.f), fCnt(0.f), fMax(0.f)
	{

	}

	tagFrame(float _frame, float _cnt, float _max)
		: fFrame(_frame), fCnt(_cnt), fMax(_max) 
	{

	}

}FRAME;

typedef struct tagData
{
	int		iHp;
	int		iMp;
	int		iAtt;
	int		iDef;

	tagData(void ){}

	tagData(int _hp, int _mp, int _att, int _def)
		: iHp(_hp), iMp(_mp), iAtt(_att), iDef(_def) {}
}DATA;