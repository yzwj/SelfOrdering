#if !defined(AFX_ORDERVIEW_H__5AACF30F_9829_40F8_82A6_1B90291E10BA__INCLUDED_)
#define AFX_ORDERVIEW_H__5AACF30F_9829_40F8_82A6_1B90291E10BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "yoda_self_orderingView.h"
class Cyoda_self_orderingView;

class COrderContainerCtrl : public CBCGPVisualContainerCtrl
{
	friend class Cyoda_self_orderingView;
	DECLARE_SERIAL(COrderContainerCtrl)
public:
	enum ORDER_MODE {
		ORDER_MODE_NONE = 0,
		ORDER_MODE_CREATE = 1,
		ORDER_MODE_MODIFY = 2,
	};
	COrderContainerCtrl();
	virtual ~COrderContainerCtrl();
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrderContainerCtrl)
	//}}AFX_VIRTUAL
	virtual BOOL OnMouseDown(int nButton, const CBCGPPoint& pt);
	// Implementation
public :
	void OnInitOrder();
	//int  GetOrderIndex() { return ++m_nCurOrderIndex; }
	void UpdateOrder();
	void UpdateCounts(CString szTotal);
	void UpdateTotalMoney(CString szMoney);
protected:
	// Generated message map functions
	//{{AFX_MSG(COrderContainerCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	Cyoda_self_orderingView*        m_pView;
	CBCGPVisualContainer*		m_pOwnerOrderView;
	ORDER_MODE					m_curOrderMode;
protected:
	UINT						m_nTotalCups;
	int							m_nCurOrderIndex;
	float						m_nTotalMoney;
	CBCGPSize					m_OrderSize;
	double						m_dbMoney;
};

#endif // !defined(AFX_TEAVIEW_H__5AACF30F_9829_40F8_82A6_1B90291E10BA__INCLUDED_)
