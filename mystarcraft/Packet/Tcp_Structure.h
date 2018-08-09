#pragma once

typedef struct _S_PT_LOBY_ENTER
{
	WCHAR USER_ID[32];
	WCHAR VIRTUAL_ADDRESS[32];
	USHORT VIRTUAL_PORT;
} S_PT_LOBY_ENTER;

typedef struct _S_PT_LOBY_ENTER_SUCC_U
{
	WCHAR USER_ID[32];
	DWORD_PTR SESSION_ID;
	WCHAR VIRTUAL_ADDRESS[32];
	USHORT VIRTUAL_PORT;
	WCHAR REAL_ADDRESS[32];
	USHORT REAL_PORT;
} S_PT_LOBY_ENTER_SUCC_U;

typedef struct _S_PT_ROOM_LEAVE
{
} S_PT_ROOM_LEAVE;

typedef struct _S_PT_ROOM_LEAVE_SUCC_U
{
} S_PT_ROOM_LEAVE_SUCC_U;

typedef struct _S_PT_ROOM_LEAVE_M
{
	DWORD_PTR SESSION_ID;
	DWORD_PTR MASTERSESSION_ID;
} S_PT_ROOM_LEAVE_M;

typedef struct _S_PT_CREATE_ROOM
{
	WCHAR TITLE[32];
} S_PT_CREATE_ROOM;

typedef struct _S_PT_CREATE_ROOM_SUCC_U
{
	USHORT SLOT_IDX;
	WCHAR TITLE[32];
} S_PT_CREATE_ROOM_SUCC_U;

typedef struct _S_PT_ROOMLIST_ERASE_M
{
	USHORT ROOM_IDX;
} S_PT_ROOMLIST_ERASE_M;

typedef struct _S_PT_ROOMLIST_RENEWAL
{
} S_PT_ROOMLIST_RENEWAL;

typedef struct _S_PT_ROOMLIST_RENEWAL_M
{
	WCHAR TITLE[32];
	DWORD_PTR MASTERSESSION_ID;
	USHORT ROOM_IDX;
} S_PT_ROOMLIST_RENEWAL_M;

typedef struct _S_PT_ROOM_GET_MASTER_INFO
{
	USHORT ROOM_IDX;
} S_PT_ROOM_GET_MASTER_INFO;

typedef struct _S_PT_ROOM_GET_MASTER_INFO_SUCC_U
{
	DWORD_PTR MASTERSESSION_ID;
} S_PT_ROOM_GET_MASTER_INFO_SUCC_U;

typedef struct _S_PT_ROOM_ENTER
{
	USHORT ROOM_IDX;
} S_PT_ROOM_ENTER;

typedef struct _S_PT_ROOM_ENTER_SUCC_U
{
	WCHAR TITLE[32];
	DWORD_PTR MASTERSESSION_ID;
} S_PT_ROOM_ENTER_SUCC_U;

typedef struct _S_PT_ROOM_USER_RENEWAL
{
	WCHAR TRIBE[32];
} S_PT_ROOM_USER_RENEWAL;

typedef struct _S_PT_ROOM_USER_RENEWAL_SUCC_U
{
	SLOT_USER_DATA ROOM_USER_INFO[8];
	DWORD_PTR MASTERSESSION_ID;
} S_PT_ROOM_USER_RENEWAL_SUCC_U;

typedef struct _S_PT_ROOM_USER_ENTRY_M
{
	WCHAR USER_ID[32];
	DWORD_PTR SESSION_ID;
	WCHAR VIRTUAL_ADDRESS[32];
	USHORT VIRTUAL_PORT;
	WCHAR REAL_ADDRESS[32];
	USHORT REAL_PORT;
	USHORT SLOT_IDX;
	WCHAR TRIBE[32];
} S_PT_ROOM_USER_ENTRY_M;

typedef struct _S_PT_ROOM_SENDCHAT
{
	WCHAR USER_ID[32];
	WCHAR MESSAGE[32];
	DWORD_PTR SESSION_ID;
} S_PT_ROOM_SENDCHAT;

typedef struct _S_PT_ROOM_RECEIVE_CHAT_M
{
	WCHAR USER_ID[32];
	WCHAR MESSAGE[128];
	DWORD_PTR SESSION_ID;
} S_PT_ROOM_RECEIVE_CHAT_M;

typedef struct _S_PT_ROOM_TRIBE_CHANGE
{
	WCHAR TRIBE[32];
	USHORT SLOTIDX;
} S_PT_ROOM_TRIBE_CHANGE;

typedef struct _S_PT_ROOM_TRIBE_CHANGE_M
{
	WCHAR TRIBE[32];
	USHORT SLOTIDX;
} S_PT_ROOM_TRIBE_CHANGE_M;

typedef struct _S_PT_LOAD_COMPLETE
{
} S_PT_LOAD_COMPLETE;

typedef struct _S_PT_LOAD_COMPLETE_SUCC_M
{
} S_PT_LOAD_COMPLETE_SUCC_M;

