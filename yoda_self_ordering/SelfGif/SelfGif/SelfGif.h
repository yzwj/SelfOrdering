// SelfGif.h : main header file for the SelfGif DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CSelfGifApp
// See SelfGif.cpp for the implementation of this class
//

class CSelfGifApp : public CWinApp
{
public:
	CSelfGifApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
