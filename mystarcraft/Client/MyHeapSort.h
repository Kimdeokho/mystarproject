#pragma once

#include "Include.h"

const int MAXHEAP_SIZE = 128*128;

template<typename T>
class CMyHeapSort
{
private:
	T	m_nodelist[MAXHEAP_SIZE];
	int		m_capacity;
	int		m_cursize;
public:
	void swap_node(const int& idx1 , const int& idx2);
	//함수포인터 고려
	void push_node(T pnode);
	T pop_node(void);
	int getsize(void);
	T Render(const int& idx);
	void Release(void);
	void Release(PATH_NODE** openidx , boost::pool<>* _pool);
public:
	CMyHeapSort();
	~CMyHeapSort();

};

template<typename T>
CMyHeapSort<T>::CMyHeapSort(void)
{
	m_cursize = 0;
	m_capacity = MAXHEAP_SIZE;

	for(int i = 0; i < MAXHEAP_SIZE; ++i)
		m_nodelist[i] = NULL;
}

template<typename T>
CMyHeapSort<T>::~CMyHeapSort(void)
{
	Release();
}

template<typename T>
void CMyHeapSort<T>::swap_node(const int& idx1 , const int& idx2)
{
	T temp = NULL;
	temp = m_nodelist[idx1];
	m_nodelist[idx1] = m_nodelist[idx2];
	m_nodelist[idx2] = temp;
}

template<typename T>
void CMyHeapSort<T>::push_node(T pnode)
{
	m_nodelist[m_cursize] = pnode;

	int curidx = m_cursize - 1;
	int parentidx = int( (curidx - 1)/2 );



	while( curidx > 0 )
	{
		if(m_nodelist[parentidx]->iCost >= m_nodelist[curidx]->iCost)
		{
			swap_node(parentidx , curidx);
			curidx = parentidx;
			parentidx = (curidx - 1) / 2;
		}
		else
			break;
	}

	++m_cursize;
}

template<typename T>
T CMyHeapSort<T>::pop_node(void)
{
	if( 0 == m_cursize)
		return NULL;

	T ptemp = m_nodelist[0];

	m_nodelist[0] = m_nodelist[m_cursize-1];
	m_nodelist[m_cursize-1] = NULL;

	int curidx = 0;
	int leftchild = curidx*2 + 1;
	int rightchild = curidx*2 + 2;

	while(true)
	{
		if(leftchild >= MAXHEAP_SIZE)
			break;
		if(m_nodelist[curidx] == NULL)
			break;

		if(m_nodelist[leftchild] != NULL &&
			m_nodelist[rightchild] == NULL)
		{
			/*왼쪽 자식 하나 뿐이라면*/

			if(m_nodelist[curidx]->iCost >= m_nodelist[leftchild]->iCost)
			{
				/*부모보다 자식의 값이 작다면*/
				swap_node(leftchild , curidx);
				curidx = leftchild;
			}
			else
				break;
		}
		else if(m_nodelist[rightchild] != NULL &&
			m_nodelist[leftchild] == NULL)
		{
			/*오른쪽 자식 하나 뿐이라면*/

			if(m_nodelist[curidx]->iCost >= m_nodelist[rightchild]->iCost)
			{
				/*부모보다 자식의 값이 작다면*/
				swap_node(rightchild , curidx);
				curidx = rightchild;
			}
			else
				break;
		}
		else if(m_nodelist[rightchild] != NULL &&
			m_nodelist[leftchild] != NULL)
		{
			/*자식 둘다 있을때*/

			if(m_nodelist[curidx]->iCost >= m_nodelist[rightchild]->iCost &&
				m_nodelist[curidx]->iCost >= m_nodelist[leftchild]->iCost)
			{
				/*두자식 모두 부모보다 작다면*/

				if(m_nodelist[leftchild]->iCost <= m_nodelist[rightchild]->iCost)
				{
					/*왼쪽이 더 작다*/
					swap_node(leftchild , curidx);
					curidx = leftchild;
				}
				else
				{
					/*오른쪽이 더 작다*/
					swap_node(rightchild , curidx);
					curidx = rightchild;
				}
			}
			else if( m_nodelist[curidx]->iCost >= m_nodelist[rightchild]->iCost )
			{
				/*왼쪽이 부모보다 크고 오른쪽 자식이 작다면*/
				swap_node(rightchild , curidx);
				curidx = rightchild;
			}
			else if( m_nodelist[curidx]->iCost >= m_nodelist[leftchild]->iCost )
			{
				/*오른쪽이 부모보다 크고 왼쪽 자식이 작다면*/
				swap_node(leftchild , curidx);
				curidx = leftchild;
			}
			else
				break;//두 자식모두 부모보다 크다면

		}
		else if(m_nodelist[rightchild] == NULL &&
			m_nodelist[leftchild] == NULL)
			break;

		leftchild = curidx*2 + 1;
		rightchild = curidx*2 + 2;
	}

	--m_cursize;

	if(ptemp == NULL)
		int a = 0;
	return ptemp;
}

template<typename T>
int CMyHeapSort<T>::getsize(void)
{
	return m_cursize;
}
template<typename T>
T CMyHeapSort<T>::Render(const int& idx)
{
	if(idx >= m_cursize)
		return NULL;

	return m_nodelist[idx];
}
template<typename T>
void CMyHeapSort<T>::Release(PATH_NODE** openidx , boost::pool<>* _pool)
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

template<typename T>
void CMyHeapSort<T>::Release(void)
{
	for(int i = 0; i < m_cursize; ++i)
	{
		//delete m_nodelist[i];
		m_nodelist[i] = NULL;
	}
	m_cursize = 0;
}