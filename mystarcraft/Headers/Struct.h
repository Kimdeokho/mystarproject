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

	//BYTE			byTerrain_ID; //어느 지형
	//BYTE			byGroup_ID; //지형의 어느그룹
	//BYTE			byGroup_sequence; //그룹의 몇번째


	BYTE			byOption;
	BYTE			byFloor; /*0층이 지하*/
	list<TERRAIN_INFO*>	terrainList;
	tagTile()
	{
		vPos = D3DXVECTOR2(0,0);
		byOption = 0;
		byFloor = 1;
	}
	~tagTile(){}

}TILE;

typedef struct tagTile_temp
{
	BYTE			byOption;
	BYTE			byFloor; /*0층이 지하*/

	tagTile_temp()
	{
		byOption = 0;
		byFloor = 1;
	}
}TILE_TEMP;

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