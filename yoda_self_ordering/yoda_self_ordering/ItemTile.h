#if !defined(AFX_ITEMTILE_H__7BBC15C6_B727_4C7C_9777_ECDBBF729D25__INCLUDED_)
#define AFX_ITEMTILE_H__7BBC15C6_B727_4C7C_9777_ECDBBF729D25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CCategoryTile : public CBCGPWinUITile  
{
public:
	CCategoryTile(const int nCategroyID,
					const CString& strName=_T(""),
					const CString& szImagePath=_T(""),
					const CBCGPColor& colorText = CBCGPColor(),
					const CBCGPBrush& brushBackground = CBCGPBrush(),
					const CBCGPColor& colorBorder = CBCGPColor(),
					const CBCGPRect& rect = CBCGPRect());
	
	virtual ~CCategoryTile();
	UINT GetCategroyID() { return m_nCategroyID; }
protected:
	virtual void OnDrawImage(CBCGPGraphicsManager* pGM, const CBCGPRect& rectImage);
	virtual CBCGPSize GetImageSize(CBCGPGraphicsManager* pGM);
	
protected:
	CBCGPImage		    m_Image;
	CBCGPRect			m_rectItem;
	int					m_nCategroyID;
};
/////////////////////////////////////////////////////////////////////////////
class CProductTile : public CBCGPWinUITile
{
public:
	CProductTile(
		const int nCategroy,
		const int nProductID, 
		const CString& strName = _T(""),
		const CString& szImagePath = _T(""),
		const CBCGPColor& colorText = CBCGPColor(),
		const CBCGPBrush& brushBackground = CBCGPBrush(),
		const CBCGPColor& colorBorder = CBCGPColor(),
		const CBCGPRect& rect = CBCGPRect());
	int GetCategroyID() { return m_nCategroyID; }
	int GetProductID() { return m_nProductID; }
	virtual ~CProductTile();
protected:
	virtual void OnDrawImage(CBCGPGraphicsManager* pGM, const CBCGPRect& rectImage);
	virtual CBCGPSize GetImageSize(CBCGPGraphicsManager* pGM);
protected:
	CBCGPImage		    m_Image;
	CBCGPRect			m_rectItem;
	int				m_nProductID;
	int				m_nCategroyID;
};
#endif // !defined(AFX_DATETIMETILE_H__7BBC15C6_B727_4C7C_9777_ECDBBF729D25__INCLUDED_)
