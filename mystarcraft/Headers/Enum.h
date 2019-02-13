#pragma once

enum TEXTYPE {TEXTYPE_SINGLE,TEXTYPE_MULTI,TEXTYPE_GENERAL};
enum SCENEID 
{
	SCENE_NONE, 
	SCENE_LOGO,
	SCENE_LOGIN, 
	SCENE_LOBY, 
	SCENE_ROOM ,
	SCENE_STAGEPRE, 
	SCENE_STAGE, 
	SCENE_TEST,
	SCENE_END,
};

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
	BTN_B_ZBUILD,
	BTN_V_ZBUILD,
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
	BTN_CANCLE,
	BTN_SCAN,
	BTN_LANDING,
	BTN_GHOST_N,
	BTN_DEFENSIVE,
	BTN_BURROW,
	BTN_UNBURROW,
	//
	BTN_T_BEW,
	BTN_T_BEA,
	BTN_T_VAW,
	BTN_T_VAP,
	BTN_T_VAS,
	BTN_T_VAH,

	BTN_T_BA0, //BA는 건물 단축키이고 숫자는 버튼위치이다
	BTN_T_BA3, 
	BTN_T_BA4, 
	BTN_T_BA5,
	BTN_T_VFC0,
	BTN_T_VFC3,

	BTN_T_VI0, //이엠피
	BTN_T_VI1, //이레디
	BTN_T_VI2, //베슬마나업

	BTN_T_VSC0,
	BTN_T_VSC1,
	
	BTN_T_VIC0,
	BTN_T_VIC1,
	BTN_T_VIC3,
	BTN_T_VIC4,

	BTN_T_VIP0,
	BTN_T_VIP1,
	//
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
	BTN_NC_MISSILE,

	BTN_DRONE, //===================저그 유닛===================
	BTN_ZERGLING,
	BTN_OVERLOAD,
	BTN_HYDRA,
	BTN_MUTAL,
	BTN_SCOURGE,
	BTN_QUEEN,
	BTN_ULTRA,
	BTN_DEFILER,
	BTN_LURKER,

	BTN_HATCHERY, //===================저그 건물===================
	BTN_LAIR,
	BTN_HIVE,
	BTN_COLONY,
	BTN_Z_GAS,
	BTN_SPWANING,
	BTN_CHAMBER,
	BTN_SUNKEN,
	BTN_SPORE,
	BTN_HYDRADEN,
	BTN_SPIRE,
	BTN_QUEEN_NEST,
	BTN_DEFILER_MOUND,
	BTN_ULTRA_CAVE,

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
	OBJ_LARVA,
	OBJ_LARVA_EGG,
	OBJ_HATCERY,
	OBJ_LAIR,
	OBJ_HIVE,
	OBJ_SPAWNING,
	OBJ_Z_GAS,
	OBJ_FCOLONY,
	OBJ_SUNKEN,
	OBJ_SPORE,
	OBJ_SPIRE,
	OBJ_CHAMBER,
	OBJ_HYDRADEN,
	OBJ_QUEEN_NEST,
	OBJ_ULTRA_CAVE,
	OBJ_DEFILER_MOUND,
	OBJ_ZERGLING,
	OBJ_DEFILER,
	OBJ_BRUDE,
	OBJ_HYDRA,
	OBJ_ULTRA,
	OBJ_LURKER,
	OBJ_MUTAL,
	OBJ_QUEEN,
	OBJ_OVERLOAD,
	OBJ_SCOURGE,

	OBJ_TANK,
	OBJ_SIEGETANK,
	OBJ_MARINE,
	OBJ_FIREBAT,
	OBJ_MEDIC,
	OBJ_GHOST,
	OBJ_GHOLATH,
	OBJ_SCV,
	OBJ_GOLIATH,
	OBJ_VULTURE,
	OBJ_MINE,
	OBJ_BATTLE,
	OBJ_WRAITH,
	OBJ_DROPSHIP,
	OBJ_VESSEL,
	OBJ_COMMAND,
	OBJ_FACTORY,
	OBJ_STARPORT,
	OBJ_T_GAS,
	OBJ_FRAGMENT,
	OBJ_COMSET,
	OBJ_NC_PART,
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
	OBJ_ETC,
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

	Z_HATCHERY,
	Z_LAIR,
	Z_HIVE,
	Z_SPWANING_POOL,
	Z_FCOLONY,
	Z_GAS,
	Z_HYDRADEN,
	Z_CHAMBER,
	Z_SUNKEN,
	Z_SPORE,
	Z_QUEEN_NEST,
	Z_SPIRE,
	Z_ULTRA_CAVE,
	Z_DEFILER_MOUND,
	BUILD_TECH_END,
};
enum UPGRADE
{
	UPG_T_STEAMPACK,
	UPG_T_MARINE_RANGE,
	UPG_T_WRAITH_CLOACKING,
	UPG_T_BIO_WEAPON,
	UPG_T_BIO_ARMOR,
	UPG_T_MECHANIC_WEAPON,
	UPG_T_MECHANIC_ARMOR,
	UPG_T_AIR_WEAPON,
	UPG_T_AIR_ARMOR,

	UPG_T_BA0, //BA는 건물 단축키이고 숫자는 버튼위치이다
	UPG_T_BA3, 
	UPG_T_BA4, 
	UPG_T_BA5, 

	UPG_T_BCN0, //핵

	UPG_T_VFC0, 
	UPG_T_VFC1, 
	UPG_T_VFC2, 
	UPG_T_VFC3, 

	UPG_T_VI0,//EMP
	UPG_T_VI1,//이레디
	UPG_T_VI2,//베슬 마나업

	UPG_T_VSC0,
	UPG_T_VSC1,

	UPG_T_VIC0,
	UPG_T_VIC1,
	UPG_T_VIC3,
	UPG_T_VIC4,

	UPG_T_VIP0,
	UPG_T_VIP1,

	UPG_Z_BH2, //버로우
	UPG_Z_BH3, //드랍업
	UPG_Z_BH4, //오버더듬이
	UPG_Z_BH5, //오버이속

	UPG_Z_BS0, //저글링 이속
	UPG_Z_BS1, //저글링 공속

	UPG_Z_BD0, //히드라 이속
	UPG_Z_BD1, //히드라 사거리
	UPG_Z_BD2, //럴커업

	UPG_Z_VQ0, //인스네어
	UPG_Z_VQ1, //브루드링

	UPG_Z_MELEE_ATT, //지상 근접공격
	UPG_Z_MISSILE_ATT, //지상 원거리공격
	UPG_Z_FLYER_ATT, //지상 원거리

	UPG_Z_GROUND_ARMOR, //지상 아머
	UPG_Z_FLYER_ARMOR, //지상 원거리
	

	UPG_END,
};
enum TRIBE
{
	TRIBE_NONE,
	TRIBE_TERRAN,
	TRIBE_ZERG,
	TRIBE_END,
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
	MOVE_NOT,
	MOVE_GROUND,
	MOVE_AIR,
};

enum ASTAR_DIR
{
	//CENTER,
	//UP,
	//DOWN,
	//LEFT,
	//RIGHT,
	//LEFT_UP,
	//RIGHT_UP,
	//LEFT_DOWN,
	//RIGHT_DOWN,
	RIGHT_DOWN,
	DOWN,
	LEFT_DOWN,
	RIGHT,
	LEFT,
	RIGHT_UP,
	UP,
	LEFT_UP,
	CENTER,
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
	TEAM_END,
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
	//BOARDING,// 드랍쉽 , 또는 가스 탑승
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
	AIM,
	GATHER,
	MUTATE,
};

enum CATEGORY
{
	CATEGORY_NONE,
	CATEGORY_UNIT,
	CATEGORY_BUILDING,
	CATEGORY_RESOURCE,
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
	DAMAGE_NONE,
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
	COM_AIR_PATHFIND,
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
	COM_USINGSKILL,
	COM_CREEP,
	COM_CC,
	COM_LARVAHATCH,
	COM_DETECTOR,
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
	ORDER_LANDING, //건물 착륙
	ORDER_BUNKER_BOARDING,
	ORDER_GET_OFF, //드랍쉽 내리기
	ORDER_USINGSKILL,//스킬쓰러 이동중
};

enum SELECT_FLAG
{
	NONE_SELECT,
	GENERAL_SELECT,
	BLINK_SELECT,
};

enum SKILL_ORDER
{
	SO_NONE,
	SO_INSTALL_MINE,
	SO_YAMATO,
	SO_NUCLEAR,
	SO_IRRADIATE,
	SO_DEFENSIVE,
	SO_BROODLING,
	SO_ENSNARE,
	SO_DRAKSWARM,
	SO_PLAGUE,
};
enum SKILL_NAME
{
	SN_NONE,
	SN_STEAMPACK,
	SN_IRRADIATE,
	SN_DEFENSIVE,	
	SN_YAMATO,
	SN_NUCLEAR,
	SN_ENSNARE,
	SN_PLAGUE,
};

enum OBJ_RACE
{
	OBJRACE_NONE,
	OBJRACE_MECHANIC,
	OBJRACE_CREATURE,
};

enum UNIT_SIZE
{
	SIZE_NONE,
	SIZE_SMALL,
	SIZE_MEDIUM,
	SIZE_LARGE,
};

enum	EXIT_FLAG
{
	LB_ROOM_CREATE, 
	LB_ROOM_JOIN , 
	LB_FLAG_NONE,
	R_ROOM_EXIT,
	R_STAGE_JOIN,
	R_STAGE_PRE,
	R_FLAG_NONE,
};

