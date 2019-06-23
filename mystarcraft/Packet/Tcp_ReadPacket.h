#pragma once

inline VOID READ_PT_LOBY_ENTER(BYTE *buffer, S_PT_LOBY_ENTER &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.USER_ID, 32);
	Stream->ReadWCHARs(parameter.VIRTUAL_ADDRESS, 32);
	Stream->ReadUSHORT(&parameter.VIRTUAL_PORT);
}

inline VOID READ_PT_LOBY_ENTER_SUCC_U(BYTE *buffer, S_PT_LOBY_ENTER_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.USER_ID, 32);
	Stream->ReadDWORD_PTR(&parameter.SESSION_ID);
	Stream->ReadWCHARs(parameter.VIRTUAL_ADDRESS, 32);
	Stream->ReadUSHORT(&parameter.VIRTUAL_PORT);
	Stream->ReadWCHARs(parameter.REAL_ADDRESS, 32);
	Stream->ReadUSHORT(&parameter.REAL_PORT);
}

inline VOID READ_PT_ROOM_LEAVE(BYTE *buffer, S_PT_ROOM_LEAVE &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_ROOM_LEAVE_SUCC_U(BYTE *buffer, S_PT_ROOM_LEAVE_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_ROOM_LEAVE_M(BYTE *buffer, S_PT_ROOM_LEAVE_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SESSION_ID);
	Stream->ReadDWORD_PTR(&parameter.MASTERSESSION_ID);
}

inline VOID READ_PT_CREATE_ROOM(BYTE *buffer, S_PT_CREATE_ROOM &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.TITLE, 32);
}

inline VOID READ_PT_CREATE_ROOM_SUCC_U(BYTE *buffer, S_PT_CREATE_ROOM_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadUSHORT(&parameter.SLOT_IDX);
	Stream->ReadWCHARs(parameter.TITLE, 32);
}

inline VOID READ_PT_ROOMLIST_ERASE_M(BYTE *buffer, S_PT_ROOMLIST_ERASE_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadUSHORT(&parameter.ROOM_IDX);
}

inline VOID READ_PT_ROOMLIST_RENEWAL(BYTE *buffer, S_PT_ROOMLIST_RENEWAL &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_ROOMLIST_RENEWAL_M(BYTE *buffer, S_PT_ROOMLIST_RENEWAL_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.TITLE, 32);
	Stream->ReadDWORD_PTR(&parameter.MASTERSESSION_ID);
	Stream->ReadInt32(&parameter.ROOM_IDX);
}

inline VOID READ_PT_ROOM_GET_MASTER_INFO(BYTE *buffer, S_PT_ROOM_GET_MASTER_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadUSHORT(&parameter.ROOM_IDX);
}

inline VOID READ_PT_ROOM_GET_MASTER_INFO_SUCC_U(BYTE *buffer, S_PT_ROOM_GET_MASTER_INFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.MASTERSESSION_ID);
}

inline VOID READ_PT_ROOM_ENTER(BYTE *buffer, S_PT_ROOM_ENTER &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadUSHORT(&parameter.ROOM_IDX);
}

inline VOID READ_PT_ROOM_ENTER_SUCC_U(BYTE *buffer, S_PT_ROOM_ENTER_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.TITLE, 32);
	Stream->ReadDWORD_PTR(&parameter.MASTERSESSION_ID);
}

inline VOID READ_PT_ROOM_USER_RENEWAL(BYTE *buffer, S_PT_ROOM_USER_RENEWAL &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.TRIBE, 32);
}

inline VOID READ_PT_ROOM_USER_RENEWAL_SUCC_U(BYTE *buffer, S_PT_ROOM_USER_RENEWAL_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadBytes((BYTE*) parameter.ROOM_USER_INFO, sizeof(SLOT_USER_DATA) * 8);
	Stream->ReadDWORD_PTR(&parameter.MASTERSESSION_ID);
}

inline VOID READ_PT_ROOM_USER_ENTRY_M(BYTE *buffer, S_PT_ROOM_USER_ENTRY_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.USER_ID, 32);
	Stream->ReadDWORD_PTR(&parameter.SESSION_ID);
	Stream->ReadWCHARs(parameter.VIRTUAL_ADDRESS, 32);
	Stream->ReadUSHORT(&parameter.VIRTUAL_PORT);
	Stream->ReadWCHARs(parameter.REAL_ADDRESS, 32);
	Stream->ReadUSHORT(&parameter.REAL_PORT);
	Stream->ReadUSHORT(&parameter.SLOT_IDX);
	Stream->ReadWCHARs(parameter.TRIBE, 32);
}

inline VOID READ_PT_ROOM_SENDCHAT(BYTE *buffer, S_PT_ROOM_SENDCHAT &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.USER_ID, 32);
	Stream->ReadWCHARs(parameter.MESSAGE, 32);
	Stream->ReadDWORD_PTR(&parameter.SESSION_ID);
	Stream->ReadFloat(&parameter.CUR_TIME);
	Stream->ReadFloat(&parameter.RETAIN_TIME);
}

inline VOID READ_PT_ROOM_RECEIVE_CHAT_M(BYTE *buffer, S_PT_ROOM_RECEIVE_CHAT_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.USER_ID, 32);
	Stream->ReadWCHARs(parameter.MESSAGE, 128);
	Stream->ReadDWORD_PTR(&parameter.SESSION_ID);
	Stream->ReadFloat(&parameter.CUR_TIME);
	Stream->ReadFloat(&parameter.RETAIN_TIME);
}

inline VOID READ_PT_ROOM_TRIBE_CHANGE(BYTE *buffer, S_PT_ROOM_TRIBE_CHANGE &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.TRIBE, 32);
	Stream->ReadUSHORT(&parameter.SLOTIDX);
}

inline VOID READ_PT_ROOM_TRIBE_CHANGE_M(BYTE *buffer, S_PT_ROOM_TRIBE_CHANGE_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs(parameter.TRIBE, 32);
	Stream->ReadUSHORT(&parameter.SLOTIDX);
}

inline VOID READ_PT_LOAD_COMPLETE(BYTE *buffer, S_PT_LOAD_COMPLETE &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PT_LOAD_COMPLETE_SUCC_M(BYTE *buffer, S_PT_LOAD_COMPLETE_SUCC_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

