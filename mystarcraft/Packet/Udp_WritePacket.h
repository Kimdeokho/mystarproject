#pragma once

inline DWORD WRITE_PU_C2S_TUNNELING(BYTE *buffer, S_PU_C2S_TUNNELING &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(parameter.SESSION_ID);
	Stream->WriteWCHARs(parameter.REMOTE_ADDRESS, 32);
	Stream->WriteUSHORT(parameter.REMOTE_PORT);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_S2C_TUNNELING_SUCC_U(BYTE *buffer, S_PU_S2C_TUNNELING_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PU_S2C_TUNNELING_FAIL_U(BYTE *buffer, S_PU_S2C_TUNNELING_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.ERROR_CODE);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_REQ_PING(BYTE *buffer, S_PU_C2C_REQ_PING &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.TICK_COUNT);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_REQ_PING_RESULT(BYTE *buffer, S_PU_C2C_REQ_PING_RESULT &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(parameter.TICK_COUNT);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_TEST(BYTE *buffer, S_PU_TEST &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PU_START_ARRANGEMENT(BYTE *buffer, S_PU_START_ARRANGEMENT &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteBytes((BYTE*) parameter.PLACE_INFO, sizeof(PLACE_DATA) * 8);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_INIT_GAME(BYTE *buffer, S_PU_INIT_GAME &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteUSHORT(parameter.SEED);
	Stream->WriteUSHORT(parameter.PLAYER_CNT);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_TURN_DATA(BYTE *buffer, S_PU_TURN_DATA &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PU_TURN_DELAY(BYTE *buffer, S_PU_TURN_DELAY &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2S_TUNNELING(BYTE *buffer, DWORD_PTR session_id,const WCHAR *remote_address, USHORT remote_port)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD_PTR(session_id);
	WCHAR _remote_address[32] = {0,};
	wcsncpy_s(_remote_address , 32 , remote_address, _TRUNCATE);
	Stream->WriteWCHARs(_remote_address, 32);
	Stream->WriteUSHORT(remote_port);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_S2C_TUNNELING_SUCC_U(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PU_S2C_TUNNELING_FAIL_U(BYTE *buffer, DWORD error_code)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(error_code);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_REQ_PING(BYTE *buffer, DWORD tick_count)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(tick_count);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_C2C_REQ_PING_RESULT(BYTE *buffer, DWORD tick_count)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteDWORD(tick_count);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_TEST(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PU_START_ARRANGEMENT(BYTE *buffer,const PLACE_DATA *place_info)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteBytes((BYTE*) place_info, sizeof(PLACE_DATA) * 8);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_INIT_GAME(BYTE *buffer, USHORT seed, USHORT player_cnt)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteUSHORT(seed);
	Stream->WriteUSHORT(player_cnt);

	return Stream->GetLength();
}

inline DWORD WRITE_PU_TURN_DATA(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

inline DWORD WRITE_PU_TURN_DELAY(BYTE *buffer)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);


	return Stream->GetLength();
}

