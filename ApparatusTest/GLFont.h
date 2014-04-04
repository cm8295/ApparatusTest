// GLFont.h: interface for the CGLFont class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLFONT_H__94AFAAB4_CDEF_4F77_8752_E58DA6216377__INCLUDED_)
#define AFX_GLFONT_H__94AFAAB4_CDEF_4F77_8752_E58DA6216377__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGLFont  
{
public:
	void Printftext(int x, int y, LPCTSTR lpszText,HFONT hFont);
	void settext(float x,float y,CString str,HFONT Font,float r,float g,float b);

	CGLFont();
	virtual ~CGLFont();

	protected:
	HFONT hFont;

};

#endif // !defined(AFX_GLFONT_H__94AFAAB4_CDEF_4F77_8752_E58DA6216377__INCLUDED_)
