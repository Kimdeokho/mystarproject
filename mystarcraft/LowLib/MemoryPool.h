#pragma once

template <class T, int ALLOC_BLOCK_SIZE = 50>
class CMemoryPool : public CMultiThreadSync<T>
{
public:
	static VOID* operator new(std::size_t allocLength)
	{
		CThreadSync Sync;

		assert(sizeof(T) == allocLength);
		assert(sizeof(T) >= sizeof(UCHAR*));

		if (!mFreePointer)
			allocBlock();

		UCHAR *ReturnPointer = mFreePointer;
		mFreePointer = *reinterpret_cast<UCHAR**>(ReturnPointer);

		return ReturnPointer;
	}

	static VOID	operator delete(VOID* deletePointer)
	{
		CThreadSync Sync;

		*reinterpret_cast<UCHAR**>(deletePointer) = mFreePointer;
		mFreePointer = static_cast<UCHAR*>(deletePointer);
	}

	static void Extingtion_pointer(void)
	{
		delete[] mFreePointer;
		mFreePointer = NULL;
	}
private:
	static VOID	allocBlock()
	{
		mFreePointer		= new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE]; //이거 언제 지우냐??

		//reinterpret_cast 비트단위로 재해석 한다(데이터를 비트 그대로 때러넣는다) 
		//해당 주소가 유효한지 검사 안함.
		UCHAR **Current = reinterpret_cast<UCHAR **>(mFreePointer);
		UCHAR *Next		= mFreePointer;

		for (INT i=0;i<ALLOC_BLOCK_SIZE-1;++i)
		{
			Next		+= sizeof(T);
			*Current	= Next;
			Current		= reinterpret_cast<UCHAR**>(Next);
		}

		*Current = 0;
	}

private:
	static UCHAR	*mFreePointer;

protected:
	~CMemoryPool()
	{
	}
};

template <class T, int ALLOC_BLOCK_SIZE>
UCHAR* CMemoryPool<T, ALLOC_BLOCK_SIZE>::mFreePointer;
