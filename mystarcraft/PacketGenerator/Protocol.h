#pragma once

typedef enum _PROTOCOL
{
	// 프로토콜의 시작을 알립니다.
	PT_VERSION	= 0x1000000,

	PT_REG_USER,
	/*
	WCHAR	:	USER_ID[32]
	WCHAR	:	USER_NAME[32]
	DWORD	:	AGE
	BYTE	:	SEX
	WCHAR	:	ADDRESS[32]
	*/
	PT_QUERY_USER,
	/*
	WCHAR	:	USER_ID[32]
	*/
	PT_QUERY_USER_RESULT,
	/*
	WCHAR	:	USER_ID[32]
	WCHAR	:	USER_NAME[32]
	DWORD	:	AGE
	BYTE	:	SEX
	WCHAR	:	ADDRESS[32]
	*/

	PT_REG_COMPUTER,
	/*
	WCHAR	:	COMPUTER_NAME[32]
	WCHAR	:	IP_ADDRESS[32]
	BYTE	:	CPU_TYPE
	DWORD	:	RAM
	DWORD	:	HDD
	*/
	PT_QUERY_COMPUTER,
	/*
	WCHAR	:	COMPUTER_NAME[32]
	*/
	PT_QUERY_COMPUTER_RESULT,
	/*
	WCHAR	:	COMPUTER_NAME[32]
	WCHAR	:	IP_ADDRESS[32]
	BYTE	:	CPU_TYPE
	DWORD	:	RAM
	DWORD	:	HDD
	*/

	PT_REG_PROGRAM,
	/*
	WCHAR	:	PROGRAM_NAME[32]
	DWORD	:	COST
	WCHAR	:	COMMENT[32]
	*/
	PT_QUERY_PROGRAM,
	/*
	WCHAR	:	PROGRAM_NAME[32]
	*/
	PT_QUERY_PROGRAM_RESULT,
	/*
	WCHAR	:	PROGRAM_NAME[32]
	DWORD	:	COST
	WCHAR	:	COMMENT[32]
	*/

	// 프로토콜 끝
	PT_END
};