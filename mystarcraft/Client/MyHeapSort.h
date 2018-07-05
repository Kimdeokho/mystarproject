#pragma once

#include "Include.h"

//const int MAXHEAP_SIZE = 128*128; //전체 타일을 훑기위한..

template<typename T>
class CMyHeapSort
{
private:
	T*	m_nodelist;//[MAXHEAP_SIZE];
	T	m_temp;
	T	m_temppop;
	int		m_capacity;
	int		m_cursize;
public:
	void swap_node(const int& idx1 , const int& idx2);
	//함수포인터 고려
	void push_node(T pnode);
	T pop_node(void);
	int getsize(void);
	bool node_full(void);
	T Render(const int& idx);
	void Release(void);
public:
	CMyHeapSort(const int& HEAPSIZE);
	~CMyHeapSort();

};

template<typename T>
CMyHeapSort<T>::CMyHeapSort(const int& HEAPSIZE)
{
	
	m_cursize = 0;
	m_capacity = HEAPSIZE;

	m_nodelist = new T[HEAPSIZE];

	for(int i = 0; i < HEAPSIZE; ++i)
		m_nodelist[i] = NULL;
}

template<typename T>
CMyHeapSort<T>::~CMyHeapSort(void)
{
	//Release();
	delete[] m_nodelist;
	m_nodelist = NULL;
}

template<typename T>
void CMyHeapSort<T>::swap_node(const int& idx1 , const int& idx2)
{
	//m_temp = NULL;
	m_temp = m_nodelist[idx1];
	m_nodelist[idx1] = m_nodelist[idx2];
	m_nodelist[idx2] = m_temp;
}

template<typename T>
bool CMyHeapSort<T>::node_full(void)
{
	if(m_cursize >= m_capacity - 1)
		return false;

	return true;
}

template<typename T>
void CMyHeapSort<T>::push_node(T pnode)
{
	//실수로 대소비교할 생각은 그냥 하지말자

	m_nodelist[m_cursize] = pnode;
	++m_cursize;
	unsigned int curidx = m_cursize - 1;
	unsigned int parentidx = int( (curidx - 1)  >> 1 ); 

	while( curidx > 0 )
	{
		if(m_nodelist[parentidx]->iCost >= m_nodelist[curidx]->iCost)
		{
			swap_node(parentidx , curidx);
			curidx = parentidx;
			parentidx = (curidx - 1) >> 1;
		}
		else
			break;
	}
}

template<typename T>
T CMyHeapSort<T>::pop_node(void)
{
	if( 0 == m_cursize)
		return NULL;

	m_temppop = m_nodelist[0];

	m_nodelist[0] = m_nodelist[m_cursize-1];
	m_nodelist[m_cursize-1] = NULL;

	int curidx = 0;
	int leftchild = curidx*2 + 1;
	int rightchild = curidx*2 + 2;

	while(true)
	{
		if(leftchild >= m_capacity ||
			rightchild >= m_capacity)
			break;
		if(m_nodelist[curidx] == NULL)
			break;

		if(m_nodelist[leftchild] != NULL )
		{
			if(m_nodelist[rightchild] != NULL)//둘다 있다
			{				
				if(m_nodelist[curidx]->iCost >= m_nodelist[rightchild]->iCost &&
					m_nodelist[curidx]->iCost >= m_nodelist[leftchild]->iCost)
				{
					/*두자식 모두 부모보다 작다면 둘중 더 작은 노드와 교환한다*/

					if(m_nodelist[leftchild]->iCost <= m_nodelist[rightchild]->iCost)/*왼쪽이 더 작다*/
					{						
						swap_node(leftchild , curidx);
						curidx = leftchild;
					}
					else/*오른쪽이 더 작다*/
					{						
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
			else
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

		}
		else if(m_nodelist[rightchild] == NULL &&
			m_nodelist[leftchild] == NULL)
			break;

		leftchild = curidx*2 + 1;
		rightchild = curidx*2 + 2;
	}

	--m_cursize;

	return m_temppop;
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
void CMyHeapSort<T>::Release(void)
{
	for(int i = 0; i < m_cursize; ++i)
	{
		//delete m_nodelist[i];
		m_nodelist[i] = NULL;
	}
	m_cursize = 0;
}