#include "StdAfx.h"
#include "Tool_MyHeapSort.h"

CTool_MyHeapSort::CTool_MyHeapSort(void)
{
	m_cursize = 0;
	m_capacity = MAXHEAP_SIZE;

	for(int i = 0; i < MAXHEAP_SIZE; ++i)
		m_nodelist[i] = NULL;
}

CTool_MyHeapSort::~CTool_MyHeapSort(void)
{
	Release();
}
void CTool_MyHeapSort::swap_node(const int& idx1 , const int& idx2)
{
	PATH_NODE* temp = NULL;
	temp = m_nodelist[idx1];
	m_nodelist[idx1] = m_nodelist[idx2];
	m_nodelist[idx2] = temp;
}
void CTool_MyHeapSort::push_node(PATH_NODE* pnode)
{
	m_nodelist[m_cursize] = pnode;

	int parentidx = int( (m_cursize - 1)/2);
	int curidx = m_cursize;

	if( 0 != m_cursize)
	{
		while(0 != curidx)
		{
			if(m_nodelist[parentidx]->iCost >= m_nodelist[curidx]->iCost)
			{
				swap_node(parentidx , curidx);
				curidx = parentidx;
				parentidx = curidx/2;
			}
			else
				break;
		}			
	}

	++m_cursize;
}
PATH_NODE* CTool_MyHeapSort::pop_node(void)
{
	if( 0 == m_cursize)
		return NULL;

	PATH_NODE* ptemp = m_nodelist[0];

	m_nodelist[0] = m_nodelist[m_cursize-1];
	m_nodelist[m_cursize-1] = NULL;

	int curidx = 0;
	int leftchild = curidx*2 + 1;
	int rightchild = curidx*2 + 2;

	while(true)
	{
		if(leftchild >= MAXHEAP_SIZE)
			break;

		if(m_nodelist[leftchild] == NULL ||
			m_nodelist[rightchild] == NULL)
			break;

		if(m_nodelist[curidx]->iCost <= m_nodelist[leftchild]->iCost &&
			m_nodelist[curidx]->iCost <= m_nodelist[rightchild]->iCost)
			break;
		else
		{
			if(m_nodelist[leftchild]->iCost <= m_nodelist[rightchild]->iCost)
			{
				swap_node(leftchild , curidx);
				curidx = leftchild;
			}
			else					
			{
				swap_node(rightchild , curidx);
				curidx = rightchild;
			}

			leftchild = curidx*2 + 1;
			rightchild = curidx*2 + 2;
		}
	}

	--m_cursize;
	return ptemp;
}
int CTool_MyHeapSort::getsize(void)
{
	return m_cursize;
}

void CTool_MyHeapSort::Release(PATH_NODE** openidx , boost::pool<>* _pool)
{
	for(int i = 0; i < m_cursize; ++i)
	{
		openidx[ m_nodelist[i]->index ] = NULL;

		_pool->free(m_nodelist[i]);
		//delete m_nodelist[i];
		m_nodelist[i] = NULL;
	}
	m_cursize = 0;
}
void CTool_MyHeapSort::Release(void)
{
	for(int i = 0; i < m_cursize; ++i)
	{
		delete m_nodelist[i];
		m_nodelist[i] = NULL;
	}
	m_cursize = 0;
}