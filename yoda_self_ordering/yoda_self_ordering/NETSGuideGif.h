#pragma once
#include "dshow.h"
// CNETSGuideGif dialog

class CNETSGuideGif : public CDialog
{
	DECLARE_DYNAMIC(CNETSGuideGif)

public:
	CNETSGuideGif(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNETSGuideGif();

// Dialog Data
	enum { IDD = IDD_DIALOG_GIF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
public:
	IGraphBuilder *pGraph;
	IMediaControl *pControl;
	IMediaEventEx *pEvent;
	IBasicVideo *pVideo;
	IBasicAudio *pAudio;
	IVideoWindow *pWindow;
	IMediaSeeking *pSeeking;
public:
	virtual BOOL OnInitDialog();
private:
	void CreateGraph();
	void DestroyGraph();
public:
	afx_msg void OnClose();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	CStatic m_staticGif;
	CString	m_szFileName;
	//CDXGraph *mFilterGraph;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
