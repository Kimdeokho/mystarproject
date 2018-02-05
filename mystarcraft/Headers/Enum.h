#pragma once

enum TEXTYPE {TEXTYPE_SINGLE,TEXTYPE_MULTI,TEXTYPE_GENERAL};
enum SCENEID {SCENE_LOGO,SCENE_STAGE,SCENE_END,};

enum UI_ID
{
	UI_SELECT,
};
enum CMD_BTN
{
	BTN_NONE,
	BTN_ATTACK,
	BTN_MOVE,
	BTN_STOP,
	BTN_B_BUILD,
	BTN_V_BUILD,
	BTN_HOLD,
	BTN_PATROL,
	BTN_STEAMPACK,
	BTN_COMAND_CENTER,
	BTN_COMSET,
	BTN_SUPPLY,
	BTN_T_GAS,
	BTN_BARRACK,
	BTN_ACADEMY,
	BTN_BUNKER,
	BTN_TURRET,
	BTN_FACTORY,
	BTN_FAC_ADDON,
	BTN_EB,
	BTN_NC_PART,//커맨드 센터 옆에 붙은거
	BTN_STARPORT,
	BTN_STAR_ADDON,
	BTN_ARMORY,
	BTN_SIENCE,
	BTN_BATTLE_ADDON,
	BTN_GHOST_ADDON,
	BTN_TAKE_OFF,
	BTN_LANDING,
	BTN_SCV, //===================테란 유닛===================
	BTN_MARINE,
	BTN_FIREBAT,
	BTN_GHOST,
	BTN_MEDIC,
	BTN_VULTURE,
	BTN_SIEGEMODE,
	BTN_SIEGETANK,
	BTN_SPIDERMINE,
	BTN_GOLIATH,
	BTN_WRAITH,
	BTN_DROPSHIP,
	BTN_BATTLE,
	BTN_VESSEL,
	BTN_NUCLEAR,

};
enum CMD_STATE
{
	CMD_BASIC,
	CMD_B_VIEW,
	CMD_V_VIEW,
	CMD_BUILDING, //SCV가 건물 짓고있을때 CMD상태
};
enum OBJID
{
	OBJ_NONE,
	OBJ_GAS,
	OBJ_MINERAL,
	OBJ_STARTBASE,
	OBJ_DRONE,
	OBJ_HACERY,
	OBJ_TANK,
	OBJ_SIEGETANK,
	OBJ_MARINE,
	OBJ_FIREBAT,
	OBJ_MEDIC,
	OBJ_GHOST,
	OBJ_SCV,
	OBJ_GOLIATH,
	OBJ_VULTURE,
	OBJ_MINE,
	OBJ_BATTLE,
	OBJ_WRAITH,
	OBJ_DROPSHIP,
	OBJ_COMMAND,
	OBJ_FACTORY,
	OBJ_STARPORT,
	OBJ_GASBUILDING,
	OBJ_FRAGMENT,
	OBJ_COMSET,
	OBJ_NUCLEAR,
	OBJ_FAC_ADDON,
	OBJ_STAR_ADDON,
	OBJ_BATTLE_ADDON,
	OBJ_GHOST_ADDON,
	OBJ_SIENCE,
	OBJ_BARRACK,
	OBJ_EB,
	OBJ_SUPPLY,
	OBJ_ACADEMY,
	OBJ_ARMOURY,
	OBJ_BUNKER,
	OBJ_TURRET,
	OBJ_BULLET,
	OBJ_FOG,
	OBJ_END,
};
enum TERRAN_BUILD_TECH
{
	BUILD_NONE,
	T_COMMANDCENTER,
	T_GAS,
	T_COMSET,
	T_NC_PART,
	T_FACTORY,
	T_STARPORT,
	T_SIENCE,
	T_FAC_ADDON,
	T_STAR_ADDON,
	T_BATTLE_ADDON,
	T_GHOST_ADDON,
	T_BARRACK,
	T_ACADEMY,
	T_SUPPLY,
	T_ARMOURY,
	T_BUNKER,
	T_EB,
	T_TURRET,
	T_BUILD_END,
};
enum UPGRADE
{
	UPG_T_STEAMPACK,
	UPG_T_MARINE_RANGE,
	UPG_T_SIEGE_MODE,
	UPG_T_SPIDER_MINE,
	UPG_T_VULTURE_SPEED,
	UPG_T_GOLIATH_RANGE,
	UPG_T_WRAITH_CLOACKING,
	UPG_T_BIO_WEAPON,
	UPG_T_BIO_ARMOR,
	UPG_T_MECHANIC_WEAPON,
	UPG_T_MECHANIC_ARMOR,
	UPG_T_AIR_WEAPON,
	UPG_T_AIR_ARMOR,
	UPG_END,
};
enum PRODUCTION_ID
{
	PRODUCTION_NONE,
	PRODUCTION_DRONE,
	PRODUCTION_TANK,
	PRODUCTION_MARINE,
	PRODUCTION_FIREBAT,
	PRODUCTION_MEDIC,
	PRODUCTION_SCV,
	PRODUCTION_GOLIATH,
	PRODUCTION_VULTURE,
	PRODUCTION_MINE,
	PRODUCTION_BATTLE,
	PRODUCTION_WRAITH,
	PRODUCTION_DROPSHIP,
	PRODUCTION_VESSEL,
	PRODUCTION_END,
};
enum RACE
{
	TERRAN,
	ZERG,
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
	SORT_CORPSE,
	SORT_GROUND,
	SORT_GROUND_EFF,
	SORT_AIR,
	SORT_AIR_EFF,
	SORT_UI,
	SORT_END,
};

enum MOVE_TYPE
{
	MOVE_GROUND,
	MOVE_AIR,
};

enum ASTAR_DIR
{
	CENTER,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	LEFT_UP,
	RIGHT_UP,
	LEFT_DOWN,
	RIGHT_DOWN,
	ASTAR_DIR_END,
};
enum TEAM_NUMBER
{
	TEAM_0,
	TEAM_1,
	TEAM_2,
	TEAM_3,
	TEAM_4,
	TEAM_5,
	TEAM_6,
	TEAM_7,
	TEAM_NONE,
};

enum STATE
{
	STATE_NONE,
	IDLE,
	MOVE,
	COLLISION, //길찾다 충돌할경우 잠시대기, (피해가기 위해)
	ATTACK,
	HEALING,
	TRANSFORMING,
	BUILD,
	BOARDING,// 드랍쉽 , 또는 가스 탑승
	TAKE_OFF,
	LANDING,
	AIR_IDLE,
	WORK,
	BURROW,
	PLANT,
	PULL,
	PRODUCTION,
	DEVELOPING, //업그레이드 개발중
	ADDING_ON,
};

enum CATEGORY
{
	UNIT,
	BUILDING,
	RESOURCE,
};

enum TARGET_SEARCH_TYPE
{
	SEARCH_ONLY_ENEMY,
	SEARCH_ONLY_ALLY,
	SEARCH_ANYTHING,//아무거나
};

enum ATTACK_SEARCH_TYPE
{
	ATTACK_ONLY_GROUND,
	ATTACK_ONLY_AIR,
	ATTACK_ANYTHING,
};

enum DAMAGE_TYPE
{
	DAMAGE_BOOM,
	DAMAGE_NOMAL,
	DAMAGE_VIBRATE,
};
enum ARMOR_TYPE
{
	ARMOR_SMALL,
	ARMOR_MEDIUM,
	ARMOR_LARGE,
};
enum COMPONENT_LIST
{
	COM_PATHFINDE,
	COM_FOG,
	COM_TARGET_SEARCH,
	COM_MULTITEX,
	COM_GENERALTEX,
	COM_SINGLETEX,
	COM_COLLISION,
	COM_AUTOSEARCH,
	COM_ANIMATION,
	COM_WEAPON,
	COM_TRANSPORT,
	COM_PRODUCTION_BUILDING,
};

enum ORDER
{
	ORDER_NONE,
	ORDER_MOVE,
	ORDER_MOVE_ATTACK,
	ORDER_GATHER, //자원캐기
	ORDER_RETURN_CARGO, //자원 센터에 쳐집어넣기
	ORDER_MOVE_BUILD,//건물 지으러 이동하기
	ORDER_BUILDING,//SCV 경우 건물 짓는 행위
	ORDER_LANDING_MOVE,
	ORDER_LANDING,
	ORDER_BUNKER_BOARDING,
	ORDER_INSTALL_MINE,
	ORDER_GET_OFF,
};

enum SELECT_FLAG
{
	NONE_SELECT,
	GENERAL_SELECT,
	BLINK_SELECT,
};
