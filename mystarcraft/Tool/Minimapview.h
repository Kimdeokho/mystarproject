#pragma once


// CMinimapview ���Դϴ�.

class CMinimapview : public CView
{
	DECLARE_DYNCREATE(CMinimapview)

protected:
	CMinimapview();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMinimapview();

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};


