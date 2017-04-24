#pragma once

enum TEXTYPE {TEXTYPE_SINGLE,TEXTYPE_MULTI,TEXTYPE_GENERAL};
enum SCENEID {SCENE_LOGO,SCENE_STAGE,SCENE_END,};

enum OBJID
{
	OBJ_GAS,
	OBJ_MINERAL,
	OBJ_STARTBASE,
	OBJ_END,
};
enum ZERG_BUILDING_ID
{
	ZB_HACHERY,
	ZB_END,
};
enum ZERG_UNIT_ID
{
	ZU_DRONE,
	ZU_END,
};

enum OBJ_POS_KIND
{
	STATIC_OBJ,
	DYNAMIC_OBJ,
};
enum DATAID
{
	DATA_PLAYER,
	DATA_PLAYERMATRIX,
	DATA_ITEM,
	DATA_MONSTERINFO,
};

enum FLOOR
{
	UP_FLOOR_1,
	UP_FLOOR_2,
	DOWN_FLOOR_1,
	DOWN_FLOOR_2,
	HILL,
	FLOOR_END,
};

enum TERRAIN_GROUP
{
	GROUP_FLAT,
	GROUP_L,
	GROUP_LU,
	GROUP_RU,
	GROUP_R,
	GROUP_RD,
	GROUP_LD,
	GROUP_LEDGE,
	GROUP_REDGE,
	GROUP_END,
};

enum HILL_DIR
{
	HILL_L,
	HILL_R,
};

enum TILE_OPTION
{
	MOVE_OK,
	MOVE_NONE,
	RESOURCE_GAS,
	RESOURCE_MINERAL,
	OP_STARTBASE,
};

enum DEBUG_OPTION
{
	DBG_GROUP,
	DBG_MOVE,
	DBG_HILL,
	DBG_GAS,
	DBG_MINERAL,
	DBG_STARTBASE,
	DBG_END,
};
enum FOGSIGHT_OPTION
{
	FOG_BLACK, /*안가본곳*/
	FOG_GREY, /*한번 가봤던곳*/
	FOG_ALPHA /*현재 보고 있는곳*/
};
enum SORT_ID
{
	SORT_GROUND,
	SORT_AIR,
	SORT_UI,
	SORT_END,
};

enum UNIT_TYPE
{
	TYPE_GROUND,
	TYPE_AIR,
};