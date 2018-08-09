#pragma once

inline VOID READ_PU_C2S_TUNNELING(BYTE *buffer, S_PU_C2S_TUNNELING &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD_PTR(&parameter.SESSION_ID);
	Stream->ReadWCHARs(parameter.REMOTE_ADDRESS, 32);
	Stream->ReadUSHORT(&parameter.REMOTE_PORT);
}

inline VOID READ_PU_S2C_TUNNELING_SUCC_U(BYTE *buffer, S_PU_S2C_TUNNELING_SUCC_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PU_S2C_TUNNELING_FAIL_U(BYTE *buffer, S_PU_S2C_TUNNELING_FAIL_U &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.ERROR_CODE);
}

inline VOID READ_PU_C2C_REQ_PING(BYTE *buffer, S_PU_C2C_REQ_PING &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.TICK_COUNT);
}

inline VOID READ_PU_C2C_REQ_PING_RESULT(BYTE *buffer, S_PU_C2C_REQ_PING_RESULT &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadDWORD(&parameter.TICK_COUNT);
}

inline VOID READ_PU_TEST(BYTE *buffer, S_PU_TEST &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

inline VOID READ_PU_START_ARRANGEMENT(BYTE *buffer, S_PU_START_ARRANGEMENT &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadBytes((BYTE*) parameter.PLACE_INFO, sizeof(PLACE_DATA) * 8);
}

inline VOID READ_PU_INIT_GAME(BYTE *buffer, S_PU_INIT_GAME &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadUSHORT(&parameter.SEED);
	Stream->ReadUSHORT(&parameter.PLAYER_CNT);
}

inline VOID READ_PU_TURN_DATA(BYTE *buffer, S_PU_TURN_DATA &parameter)
{
	CStreamSP Stream;
	Stream->SetBuffer(buffer);

}

