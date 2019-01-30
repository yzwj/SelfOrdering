// yoda_self_ordering_cn.h : main header file for the yoda_self_ordering_cn DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cyoda_self_ordering_cnApp
// See yoda_self_ordering_cn.cpp for the implementation of this class
//

class Cyoda_self_ordering_cnApp : public CWinApp
{
public:
	Cyoda_self_ordering_cnApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
