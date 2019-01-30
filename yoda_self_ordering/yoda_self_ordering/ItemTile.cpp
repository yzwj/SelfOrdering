#include "stdafx.h"
#include "yoda_self_ordering.h"
#include "ItemTile.h"

#include "yoda_self_orderingDoc.h"
#include "yoda_self_orderingView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCategoryTile::CCategoryTile(
	const int nCategroyID, 
	const CString& strName,
	const CString& szImagePath,
	const CBCGPColor& colorText,
	const CBCGPBrush& brushBackground,
	const CBCGPColor& colorBorder,
	const CBCGPRect& rect ) :
	CBCGPWinUITile(strName, TRUE /* wide */, colorText, brushBackground, colorBorder)
{
	m_Image = CBCGPImage(szImagePath);
	m_nCategroyID = nCategroyID;
}

CCategoryTile::~CCategoryTile()
{
}

void CCategoryTile::OnDrawImage(CBCGPGraphicsManager* pGM, const CBCGPRect& rectImage)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pGM);

	CBCGPSize sizeImage = GetImageSize(pGM);
	if (!sizeImage.IsEmpty())
	{
		double xOffset = max(0, 0.5 * (rectImage.Width() - sizeImage.cx));
		double yOffset = 0;

		switch (GetImageVertAlign())
		{
		case BCGP_WINUI_IMAGE_ALIGNMENT_LEADING:
			break;

		case BCGP_WINUI_IMAGE_ALIGNMENT_CENTER:
			yOffset = max(0, 0.5 * (rectImage.Height() - sizeImage.cy));
			break;

		case BCGP_WINUI_IMAGE_ALIGNMENT_TRAILING:
			yOffset = max(0, rectImage.Height() - sizeImage.cy);
			break;
		}

		pGM->DrawImage(m_Image, rectImage.TopLeft() + CBCGPPoint(xOffset, yOffset));
	}
	//pGM->DrawImage(m_Image, rectImage.TopLeft());
}

CBCGPSize CCategoryTile::GetImageSize(CBCGPGraphicsManager* pGM)
{
	CBCGPSize size = m_Image.GetSize(pGM);
	if (size.cx > m_rect.Width() || size.cy >= m_rect.Height())
	{
		
		m_Image.Resize(CBCGPSize(m_rect.Width(), m_rect.Height()));
		return CBCGPSize(m_rect.Width(), m_rect.Height());
	}
	
	return size;
}


CProductTile::CProductTile(
	const int nCategroy,
	const int nProductID,
	const CString& strName,
	const CString& szImagePath,
	const CBCGPColor& colorText,
	const CBCGPBrush& brushBackground,
	const CBCGPColor& colorBorder,
	const CBCGPRect& rect) :
	CBCGPWinUITile(strName, TRUE /* wide */, colorText, brushBackground, colorBorder)
{
	m_Image = CBCGPImage(szImagePath);
	m_rect = rect;
	m_nCategroyID = nCategroy;
	m_nProductID = nProductID;
}

CProductTile::~CProductTile()
{
}

void CProductTile::OnDrawImage(CBCGPGraphicsManager* pGM, const CBCGPRect& rectImage)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pGM);

	CBCGPSize sizeImage = GetImageSize(pGM);
	if (!sizeImage.IsEmpty())
	{
		double xOffset = max(0, 0.5 * (rectImage.Width() - sizeImage.cx));
		double yOffset = 0;

		switch (GetImageVertAlign())
		{
		case BCGP_WINUI_IMAGE_ALIGNMENT_LEADING:
			break;

		case BCGP_WINUI_IMAGE_ALIGNMENT_CENTER:
			yOffset = max(0, 0.5 * (rectImage.Height() - sizeImage.cy));
			break;

		case BCGP_WINUI_IMAGE_ALIGNMENT_TRAILING:
			yOffset = max(0, rectImage.Height() - sizeImage.cy);
			break;
		}

		pGM->DrawImage(m_Image, rectImage.TopLeft() + CBCGPPoint(xOffset, yOffset));
	}
}

CBCGPSize CProductTile::GetImageSize(CBCGPGraphicsManager* pGM)
{
	CBCGPSize size = m_Image.GetSize(pGM);
	if (size.cx > m_rect.Width() || size.cy >= m_rect.Height())
	{
		m_Image.Resize(CBCGPSize(m_rect.Width(), m_rect.Height()));
		return CBCGPSize(m_rect.Width(), m_rect.Height());
	}

	return size;
}