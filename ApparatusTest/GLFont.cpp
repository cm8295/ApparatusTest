// GLFont.cpp: implementation of the CGLFont class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GLFont.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGLFont::CGLFont()
{

}

CGLFont::~CGLFont()
{

}


void CGLFont:: settext (float x,float y,CString str,HFONT Font,float r,float g,float b)
  
{   glLoadIdentity();
	glPushAttrib(GL_CURRENT_BIT);
	glDisable(GL_TEXTURE_2D); 
	glDisable(GL_LIGHTING); 
	glColor3f(r,g,b);  
	  glTranslatef(-(420-x)/800,(260-y)/600,-1.0f);
	  Printftext (0,0, str,Font);    
	glEnable(GL_LIGHTING);     
	glEnable(GL_TEXTURE_2D);
	glPopAttrib();
}
void CGLFont:: Printftext (int x, int y, LPCTSTR lpszText,HFONT hFont)
{ CBitmap bitmap;
  BITMAP bm;
  SIZE size; 
  HDC MDC = ::CreateCompatibleDC(0);
  SelectObject(MDC,hFont);	
  ::GetTextExtentPoint32(MDC,lpszText,strlen(lpszText),&size);
  bitmap.CreateBitmap(size.cx, size.cy, 1, 1, NULL);
  HBITMAP oldBmp=(HBITMAP)SelectObject(MDC,bitmap);
  SetBkColor  (MDC, RGB(0,     0,   0));
  SetTextColor(MDC, RGB(255, 255, 255));
  TextOut(MDC, 0, 0, lpszText, strlen(lpszText));
  bitmap.GetBitmap(&bm);
  size.cx = (bm.bmWidth + 31) & (~31);
  int bufsize =size.cy * size.cx;
  struct {  BITMAPINFOHEADER bih;
			RGBQUAD col[2];
 		 }bic; 
  BITMAPINFO *binf = (BITMAPINFO *)&bic; 
  binf->bmiHeader.biSize     = sizeof(binf->bmiHeader);//
  binf->bmiHeader.biWidth    = bm.bmWidth;
  binf->bmiHeader.biHeight   = bm.bmHeight;
  binf->bmiHeader.biPlanes   = 1;   
  binf->bmiHeader.biBitCount = 1;
  binf->bmiHeader.biCompression = BI_RGB;
  binf->bmiHeader.biSizeImage   = bufsize; 
  UCHAR* Bits = new UCHAR[bufsize];	
  ::GetDIBits(MDC,bitmap,0,bm.bmHeight,Bits,binf,DIB_RGB_COLORS); 
                                      
  glPixelStorei(GL_UNPACK_ALIGNMENT ,1);
  glRasterPos2i(x,y); 
  glBitmap(size.cx,size.cy,0,0,0,0,Bits); 
  delete Bits;    
  SelectObject(MDC, oldBmp);  
  ::DeleteDC(MDC);
}