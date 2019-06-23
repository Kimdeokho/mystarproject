#pragma once

inline DWORD WRITE_PT_LOBY_ENTER(BYTE *buffer, S_PT_LOBY_ENTER &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.USER_ID, 32);
	Stream->WriteWCHARs(parameter.VIRTUAL_ADDRESS, 32);
	Stream->WriteUSHORT(parameter.VIRTUAL_PORT);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_LOBY_ENTER_SUCC_U(BYTE *buffer, S_PT_LOBY_ENTER_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.USER_ID, 32);
	Stream->WriteDWORD_PTR(parameter.SESSION_ID);
	Stream->WriteWCHARs(parameter.VIRTUAL_ADDRESS, 32);
	Stream->WriteUSHORT(parameter.VIRTUAL_PORT);
	Stream->WriteWCHARs(parameter.REAL_ADDRESS, 32);
	Stream->WriteUSHORT(parameter.REAL_PORT);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_LEAVE(BYTE *buffer, S_PT_ROOM_LEAVE &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_LEAVE_SUCC_U(BYTE *buffer, S_PT_ROOM_LEAVE_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_LEAVE_M(BYTE *buffer, S_PT_ROOM_LEAVE_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SESSION_ID);
	Stream->WriteDWORD_PTR(parameter.MASTERSESSION_ID);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CREATE_ROOM(BYTE *buffer, S_PT_CREATE_ROOM &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.TITLE, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CREATE_ROOM_SUCC_U(BYTE *buffer, S_PT_CREATE_ROOM_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteUSHORT(parameter.SLOT_IDX);
	Stream->WriteWCHARs(parameter.TITLE, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOMLIST_ERASE_M(BYTE *buffer, S_PT_ROOMLIST_ERASE_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteUSHORT(parameter.ROOM_IDX);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOMLIST_RENEWAL(BYTE *buffer, S_PT_ROOMLIST_RENEWAL &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOMLIST_RENEWAL_M(BYTE *buffer, S_PT_ROOMLIST_RENEWAL_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.TITLE, 32);
	Stream->WriteDWORD_PTR(parameter.MASTERSESSION_ID);
	Stream->WriteInt32(parameter.ROOM_IDX);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_GET_MASTER_INFO(BYTE *buffer, S_PT_ROOM_GET_MASTER_INFO &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteUSHORT(parameter.ROOM_IDX);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_GET_MASTER_INFO_SUCC_U(BYTE *buffer, S_PT_ROOM_GET_MASTER_INFO_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.MASTERSESSION_ID);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_ENTER(BYTE *buffer, S_PT_ROOM_ENTER &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteUSHORT(parameter.ROOM_IDX);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_ENTER_SUCC_U(BYTE *buffer, S_PT_ROOM_ENTER_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.TITLE, 32);
	Stream->WriteDWORD_PTR(parameter.MASTERSESSION_ID);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_USER_RENEWAL(BYTE *buffer, S_PT_ROOM_USER_RENEWAL &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.TRIBE, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_USER_RENEWAL_SUCC_U(BYTE *buffer, S_PT_ROOM_USER_RENEWAL_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteBytes((BYTE*) parameter.ROOM_USER_INFO, sizeof(SLOT_USER_DATA) * 8);
	Stream->WriteDWORD_PTR(parameter.MASTERSESSION_ID);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_USER_ENTRY_M(BYTE *buffer, S_PT_ROOM_USER_ENTRY_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.USER_ID, 32);
	Stream->WriteDWORD_PTR(parameter.SESSION_ID);
	Stream->WriteWCHARs(parameter.VIRTUAL_ADDRESS, 32);
	Stream->WriteUSHORT(parameter.VIRTUAL_PORT);
	Stream->WriteWCHARs(parameter.REAL_ADDRESS, 32);
	Stream->WriteUSHORT(parameter.REAL_PORT);
	Stream->WriteUSHORT(parameter.SLOT_IDX);
	Stream->WriteWCHARs(parameter.TRIBE, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_SENDCHAT(BYTE *buffer, S_PT_ROOM_SENDCHAT &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.USER_ID, 32);
	Stream->WriteWCHARs(parameter.MESSAGE, 32);
	Stream->WriteDWORD_PTR(parameter.SESSION_ID);
	Stream->WriteFloat(parameter.CUR_TIME);
	Stream->WriteFloat(parameter.RETAIN_TIME);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_RECEIVE_CHAT_M(BYTE *buffer, S_PT_ROOM_RECEIVE_CHAT_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.USER_ID, 32);
	Stream->WriteWCHARs(parameter.MESSAGE, 128);
	Stream->WriteDWORD_PTR(parameter.SESSION_ID);
	Stream->WriteFloat(parameter.CUR_TIME);
	Stream->WriteFloat(parameter.RETAIN_TIME);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_TRIBE_CHANGE(BYTE *buffer, S_PT_ROOM_TRIBE_CHANGE &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.TRIBE, 32);
	Stream->WriteUSHORT(parameter.SLOTIDX);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_TRIBE_CHANGE_M(BYTE *buffer, S_PT_ROOM_TRIBE_CHANGE_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs(parameter.TRIBE, 32);
	Stream->WriteUSHORT(parameter.SLOTIDX);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_LOAD_COMPLETE(BYTE *buffer, S_PT_LOAD_COMPLETE &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_LOAD_COMPLETE_SUCC_M(BYTE *buffer, S_PT_LOAD_COMPLETE_SUCC_M &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_LOBY_ENTER(BYTE *buffer,const WCHAR *user_id,const WCHAR *virtual_address, USHORT virtual_port)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _user_id[32] = {0,};
	wcsncpy_s(_user_id , 32 , user_id, _TRUNCATE);
	Stream->WriteWCHARs(_user_id, 32);
	WCHAR _virtual_address[32] = {0,};
	wcsncpy_s(_virtual_address , 32 , virtual_address, _TRUNCATE);
	Stream->WriteWCHARs(_virtual_address, 32);
	Stream->WriteUSHORT(virtual_port);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_LOBY_ENTER_SUCC_U(BYTE *buffer,const WCHAR *user_id, DWORD_PTR session_id,const WCHAR *virtual_address, USHORT virtual_port,const WCHAR *real_address, USHORT real_port)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _user_id[32] = {0,};
	wcsncpy_s(_user_id , 32 , user_id, _TRUNCATE);
	Stream->WriteWCHARs(_user_id, 32);
	Stream->WriteDWORD_PTR(session_id);
	WCHAR _virtual_address[32] = {0,};
	wcsncpy_s(_virtual_address , 32 , virtual_address, _TRUNCATE);
	Stream->WriteWCHARs(_virtual_address, 32);
	Stream->WriteUSHORT(virtual_port);
	WCHAR _real_address[32] = {0,};
	wcsncpy_s(_real_address , 32 , real_address, _TRUNCATE);
	Stream->WriteWCHARs(_real_address, 32);
	Stream->WriteUSHORT(real_port);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_LEAVE(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_LEAVE_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_LEAVE_M(BYTE *buffer, DWORD_PTR session_id, DWORD_PTR mastersession_id)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(session_id);
	Stream->WriteDWORD_PTR(mastersession_id);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CREATE_ROOM(BYTE *buffer,const WCHAR *title)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _title[32] = {0,};
	wcsncpy_s(_title , 32 , title, _TRUNCATE);
	Stream->WriteWCHARs(_title, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_CREATE_ROOM_SUCC_U(BYTE *buffer, USHORT slot_idx,const WCHAR *title)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteUSHORT(slot_idx);
	WCHAR _title[32] = {0,};
	wcsncpy_s(_title , 32 , title, _TRUNCATE);
	Stream->WriteWCHARs(_title, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOMLIST_ERASE_M(BYTE *buffer, USHORT room_idx)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteUSHORT(room_idx);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOMLIST_RENEWAL(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOMLIST_RENEWAL_M(BYTE *buffer,const WCHAR *title, DWORD_PTR mastersession_id, INT room_idx)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _title[32] = {0,};
	wcsncpy_s(_title , 32 , title, _TRUNCATE);
	Stream->WriteWCHARs(_title, 32);
	Stream->WriteDWORD_PTR(mastersession_id);
	Stream->WriteInt32(room_idx);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_GET_MASTER_INFO(BYTE *buffer, USHORT room_idx)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteUSHORT(room_idx);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_GET_MASTER_INFO_SUCC_U(BYTE *buffer, DWORD_PTR mastersession_id)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(mastersession_id);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_ENTER(BYTE *buffer, USHORT room_idx)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteUSHORT(room_idx);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_ENTER_SUCC_U(BYTE *buffer,const WCHAR *title, DWORD_PTR mastersession_id)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _title[32] = {0,};
	wcsncpy_s(_title , 32 , title, _TRUNCATE);
	Stream->WriteWCHARs(_title, 32);
	Stream->WriteDWORD_PTR(mastersession_id);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_USER_RENEWAL(BYTE *buffer,const WCHAR *tribe)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _tribe[32] = {0,};
	wcsncpy_s(_tribe , 32 , tribe, _TRUNCATE);
	Stream->WriteWCHARs(_tribe, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_USER_RENEWAL_SUCC_U(BYTE *buffer,const SLOT_USER_DATA *room_user_info, DWORD_PTR mastersession_id)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteBytes((BYTE*) room_user_info, sizeof(SLOT_USER_DATA) * 8);
	Stream->WriteDWORD_PTR(mastersession_id);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_USER_ENTRY_M(BYTE *buffer,const WCHAR *user_id, DWORD_PTR session_id,const WCHAR *virtual_address, USHORT virtual_port,const WCHAR *real_address, USHORT real_port, USHORT slot_idx,const WCHAR *tribe)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _user_id[32] = {0,};
	wcsncpy_s(_user_id , 32 , user_id, _TRUNCATE);
	Stream->WriteWCHARs(_user_id, 32);
	Stream->WriteDWORD_PTR(session_id);
	WCHAR _virtual_address[32] = {0,};
	wcsncpy_s(_virtual_address , 32 , virtual_address, _TRUNCATE);
	Stream->WriteWCHARs(_virtual_address, 32);
	Stream->WriteUSHORT(virtual_port);
	WCHAR _real_address[32] = {0,};
	wcsncpy_s(_real_address , 32 , real_address, _TRUNCATE);
	Stream->WriteWCHARs(_real_address, 32);
	Stream->WriteUSHORT(real_port);
	Stream->WriteUSHORT(slot_idx);
	WCHAR _tribe[32] = {0,};
	wcsncpy_s(_tribe , 32 , tribe, _TRUNCATE);
	Stream->WriteWCHARs(_tribe, 32);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_SENDCHAT(BYTE *buffer,const WCHAR *user_id,const WCHAR *message, DWORD_PTR session_id, FLOAT cur_time, FLOAT retain_time)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _user_id[32] = {0,};
	wcsncpy_s(_user_id , 32 , user_id, _TRUNCATE);
	Stream->WriteWCHARs(_user_id, 32);
	WCHAR _message[32] = {0,};
	wcsncpy_s(_message , 32 , message, _TRUNCATE);
	Stream->WriteWCHARs(_message, 32);
	Stream->WriteDWORD_PTR(session_id);
	Stream->WriteFloat(cur_time);
	Stream->WriteFloat(retain_time);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_RECEIVE_CHAT_M(BYTE *buffer,const WCHAR *user_id,const WCHAR *message, DWORD_PTR session_id, FLOAT cur_time, FLOAT retain_time)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _user_id[32] = {0,};
	wcsncpy_s(_user_id , 32 , user_id, _TRUNCATE);
	Stream->WriteWCHARs(_user_id, 32);
	WCHAR _message[128] = {0,};
	wcsncpy_s(_message , 128 , message, _TRUNCATE);
	Stream->WriteWCHARs(_message, 128);
	Stream->WriteDWORD_PTR(session_id);
	Stream->WriteFloat(cur_time);
	Stream->WriteFloat(retain_time);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_TRIBE_CHANGE(BYTE *buffer,const WCHAR *tribe, USHORT slotidx)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _tribe[32] = {0,};
	wcsncpy_s(_tribe , 32 , tribe, _TRUNCATE);
	Stream->WriteWCHARs(_tribe, 32);
	Stream->WriteUSHORT(slotidx);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_ROOM_TRIBE_CHANGE_M(BYTE *buffer,const WCHAR *tribe, USHORT slotidx)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	WCHAR _tribe[32] = {0,};
	wcsncpy_s(_tribe , 32 , tribe, _TRUNCATE);
	Stream->WriteWCHARs(_tribe, 32);
	Stream->WriteUSHORT(slotidx);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_LOAD_COMPLETE(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PT_LOAD_COMPLETE_SUCC_M(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

