#pragma once

#include "Include.h"

const int MAXHEAP_SIZE = 2000;

class CTool_MyHeapSort
{
private:
	PATH_NODE*	m_nodelist[MAXHEAP_SIZE];
	int		m_capacity;
	int		m_cursize;
public:
	void swap_node(const int& idx1 , const int& idx2);
	//함수포인터 고려
	void push_node(PATH_NODE* pnode);
	PATH_NODE* pop_node(void);
	int getsize(void);
	void Release(void);
	void Release(PATH_NODE** openidx , boost::pool<>* _pool);
public:
	CTool_MyHeapSort();
	~CTool_MyHeapSort();

};