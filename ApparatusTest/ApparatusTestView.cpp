
// ApparatusTestView.cpp : CApparatusTestView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ApparatusTest.h"
#endif

#include "ApparatusTestDoc.h"
#include "ApparatusTestView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool temp = false;         //temp = true:       启动手动漫游    初始化为关闭
bool tempTimer = false;    //tempTimer = true:  启动自动漫游    初始化为关闭
CString test;

// CApparatusTestView

IMPLEMENT_DYNCREATE(CApparatusTestView, CView)

BEGIN_MESSAGE_MAP(CApparatusTestView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_COMMAND(ID_32771, &CApparatusTestView::On32771)   //手动漫游
	ON_COMMAND(ID_32772, &CApparatusTestView::On32772)   //自动漫游
	ON_COMMAND(ID_32774, &CApparatusTestView::On32774)   //暂停
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_32775, &CApparatusTestView::On32775)
END_MESSAGE_MAP()

// CApparatusTestView 构造/析构

CApparatusTestView::CApparatusTestView()
{
	// TODO: 在此处添加构造代码	
}

CApparatusTestView::~CApparatusTestView()
{
	//CleanUp();
}

BOOL CApparatusTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style |=WS_CLIPCHILDREN | WS_CLIPSIBLINGS;     
	/////////////////////////////////////////////
	return CView::PreCreateWindow(cs);
}

// CApparatusTestView 绘制

void CApparatusTestView::OnDraw(CDC* /*pDC*/)
{
	CApparatusTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
			
	// TODO: 在此处为本机数据添加绘制代码
	m_ElapsedTime = GetTickCount();
	if ( ElapsedTimeinMSSinceLastRender() < 30 )
        return
		
	RenderScene();	
	::SwapBuffers(m_pDC->GetSafeHdc());	
	
	PostRenderScene();
	m_previousElapsedTime = m_ElapsedTime;
}

void CApparatusTestView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{	
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CApparatusTestView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CApparatusTestView 诊断

#ifdef _DEBUG
void CApparatusTestView::AssertValid() const
{
	CView::AssertValid();
}

void CApparatusTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CApparatusTestDoc* CApparatusTestView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CApparatusTestDoc)));
	return (CApparatusTestDoc*)m_pDocument;
}
#endif //_DEBUG



// CApparatusTestView 消息处理程序


bool CApparatusTestView::init(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);						
	glClearDepth(1.0f);											
	glDepthFunc(GL_LEQUAL);										
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);	
	glShadeModel(GL_SMOOTH);									
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	// 初始化地形 
	if(!m_Terrain.init())
	{
		
		::MessageBox(NULL,_T("初始化地形失败!"),_T("错误"),MB_OK);
		exit(0);
	}

	// 初始化天空
	if(!m_SkyBox.init())
	{
		::MessageBox(NULL,_T("初始化天空失败!"),_T("错误"),MB_OK);
		exit(0);
	}


	// 初始化3DS文件 
	m_3DS.Init("model.3DS");

     //	m_Terrain.CustomizePath();
    // 设置摄像机 
	m_Camera.setSpeed(2.0f);
	m_Camera.setCamera( 381, 35, 674,  374.5, 35, 669,  0, 1, 0);
	//参数设置
	CMenu * menu = new CMenu;
	menu->LoadMenuA(MAKEINTRESOURCE(IDD_DIALOG1));
	this->SetMenu(menu);
	
	//dirvec = m_Camera.getView() - m_Camera.getPosition();
	return true;                                        /**< 成功返回 */
}


void CApparatusTestView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	
	if(temp == true)
	{
	switch(nChar)
	{
	  case 'W':
	  case 'w':
      	   m_Camera.ForwardOrBackward(m_Camera.getSpeed());break;
	  case 'S':
	  case 's':
		   m_Camera.ForwardOrBackward(-m_Camera.getSpeed());break;
	  case 'A':
	  case 'a':
		   m_Camera.LeftOrRight(-m_Camera.getSpeed());break;
	  case 'D':
	  case 'd':
		   m_Camera.LeftOrRight(m_Camera.getSpeed());break;
	  case VK_LEFT:
		  {
		   float angleY = 1.0f;	
		   m_Camera.rotate(angleY,0,1,0);
		   break;
		  }
	  case VK_RIGHT:
		  {
		   float angleY = 1.0f;
		   m_Camera.rotate(-angleY,0,1,0);
		   break;
		  }
	  case VK_UP:
		  {
		   Vector3 dirVector,vAxis;
		   dirVector = m_Camera.getView()-m_Camera.getPosition();
		   vAxis = dirVector.crossProduct(m_Camera.getUpVector());
		   float angleZ = 0.05f;
		   m_Camera.rotate(angleZ,vAxis.x,vAxis.y,vAxis.z);
		   break;
		  }
	  case VK_DOWN:
		  {
		   Vector3 dirVector,vAxis;
		   dirVector = m_Camera.getView()-m_Camera.getPosition();
		   vAxis = dirVector.crossProduct(m_Camera.getUpVector());
		   float angleZ = 0.05f;
		   m_Camera.rotate(-angleZ,vAxis.x,vAxis.y,vAxis.z);
		   break;
		  }
	  default:break;
		
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}




void CApparatusTestView::Show3DS(float x, float z, float scale)
{
	glPushMatrix();
	float y = m_Terrain.getAveHeight(x,z) ;  // 获得此处地形高度 
	glTranslatef(x,y,z);
	glScalef(scale,scale,scale);
	m_3DS.Draw();                            // 显示3DS模型 
	glPopMatrix();
}


void CApparatusTestView::RenderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
	glLoadIdentity();
	
	// 放置摄像机 	
	m_Camera.setLook();
	
	// 渲染地形 
	m_Terrain.render();
	
	// 绘制天空 
	m_SkyBox.render();

	// 显示3DS模型 
	Show3DS(260,583,20);
	

	//显示窗体文字
    //CreateFontFun();
	
	glFlush();	              
	
}


bool CApparatusTestView::SetupPixelFormat(void)
{
	PIXELFORMATDESCRIPTOR pfd = { 
	    sizeof(PIXELFORMATDESCRIPTOR),    // struct size
	    1,                                // and its version 
	    PFD_DRAW_TO_WINDOW |              // drawing window is supported 
	    PFD_SUPPORT_OPENGL |              // OpenGL is supported 
	    PFD_DOUBLEBUFFER,                 // utilize double buffer
	    PFD_TYPE_RGBA,                    // utilize RGBA color mode 
	    24,                               // 24 bits color depth
	    0, 0, 0, 0, 0, 0,                 // ignore color bits 
	    0,                                // ignore opaque buffer 
	    0,                                //  
	    0,                                // no collective buffer 
	    0, 0, 0, 0,                       // ignore collective bits 
	    32,                               // 32 bits depth buffer     
	    0,                                // 
	    0,                                // 
	    PFD_MAIN_PLANE,                   // primary layer 
	    0,                                // reserved 
	    0, 0, 0                           // ignore layers,visibility and destruction mask 
	}; 	
	int pixelformat;
	pixelformat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);//select the specified pixel format
	::SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd);	 //setup the selected pixel format 

	return true;
}


int CApparatusTestView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_pDC = new CClientDC(this);
	InitializeOpenGL(m_pDC);
	init();	
	SetTimer(1, 0.0001, NULL);
	return 0;
}


bool CApparatusTestView::InitializeOpenGL(CDC* pDC)
{
	m_pDC = pDC;
	SetupPixelFormat();
//////////////////////////////////////////////////////////
//create rendering context
	m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc());
//////////////////////////////////////////////////////////
//make device and rendering context installed into current thread 
	::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	return true;
}


void CApparatusTestView::OnDestroy()
{
	CView::OnDestroy();

	::wglMakeCurrent(0,0);
	::wglDeleteContext( m_hRC);
	if ( m_pDC )
	{
		delete m_pDC;
	}
}


void CApparatusTestView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	glViewport(0, 0, cx, cy);									
	glMatrixMode(GL_PROJECTION);									
	glLoadIdentity();													
	gluPerspective(45.0f, (GLfloat)cx / (GLfloat)cy, 1.0f, 4000.0f);	
	glMatrixMode(GL_MODELVIEW);											
	glLoadIdentity();
}


void CApparatusTestView::OnTimer(UINT_PTR nIDEvent)
{
	
	//static int index = 0;
	//FetchPathPackage tmp(true);
	//while((tmp = m_Terrain.GetCurrentPos(index)).validation)
	//{
	//	m_Camera.setCamera(tmp.pos.x,tmp.pos.y,tmp.pos.z,tmp.view.x,tmp.view.y,tmp.view.z,0,1,0);
	//}
    Invalidate(false);	
	Vector3 vPos = m_Camera.getPosition();             
    Vector3 vNewPos = vPos; 
	vNewPos.y = (float)m_Terrain.getAveHeight(vPos.x,vPos.z ) + 10;
	float temp = vNewPos.y - vPos.y;
	Vector3 vView = m_Camera.getView();
	vView.y += temp;
	m_Camera.setCamera(vNewPos.x,  vNewPos.y,  vNewPos.z,
						   vView.x,	   vView.y,	   vView.z,	 
						   0, 1, 0);	

	 ///自动漫游部分代码
	if(tempTimer == true)
	{
		m_Camera.ForwardOrBackward(m_Camera.getSpeed());   
		float angleY = 0.06f;	
	    m_Camera.rotate(angleY,0,1,0);
	 ///
	}
	CView::OnTimer(nIDEvent);
}






///手动漫游
void CApparatusTestView::On32771()
{
	// TODO: Add your command handler code here
	SetTimer(1,0.0001,NULL);
	temp = true;
	tempTimer = false;
}

///自动漫游
void CApparatusTestView::On32772()
{
	// TODO: Add your command handler code here	
	temp = false;
	tempTimer = true;
	SetTimer(1,0.0001,NULL);
	m_Camera.ForwardOrBackward(m_Camera.getSpeed());  
	
}

///暂停漫游
void CApparatusTestView::On32774()
{
	// TODO: Add your command handler code here
	KillTimer(1);
}

///On_MouseUp按下
void CApparatusTestView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//MessageBox(TEXT("nihao"));
	


	CView::OnLButtonUp(nFlags, point);
}

//参数设置
void CApparatusTestView::On32775()
{
	// TODO: Add your command handler code here
	CDialog * dlg = new CDialog;
	dlg->Create(MAKEINTRESOURCE(IDD_DIALOG1));
	dlg->ShowWindow(1);
}


//////////////////////////////////////////////////////////////////////////////////////
///////    一下函数         ///////////////////////////////////
///////////////////////////////////////////////////

//创建字体

void CApparatusTestView::CreateFontFun()
{
	CWindowDC dc(NULL);
CFont myFont;
	myFont.CreateFont (   
120, // nHeight 字体高度  
40, // nWidth 字体宽度  
0, // nEscapement 显示角度  
0, // nOrientation 字体角度  
FW_NORMAL, // nWeight 字体磅值  
FALSE, // bItalic 是否倾斜  
FALSE, // bUnderline 是否斜体  
0, // cStrikeOut 是否加删除线  
ANSI_CHARSET, // nCharSet 指定字符集  
OUT_DEFAULT_PRECIS, // nOutPrecision 指定输出精度  
CLIP_DEFAULT_PRECIS, // nClipPrecision 指定剪切精度  
DEFAULT_QUALITY, // nQuality 指定输出质量  
DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 字符族  
"Arial"); // 指定字体的字样名  
 //CFont* oldFont = dc.SelectObject ( &myFont);  
    dc.SetTextColor (RGB (0,0,255));  
    dc.SetBkMode (TRANSPARENT); //背景模式为透明  
//
    DWORD Dura = (GetTickCount() - m_Time)/1000;	
	if(Dura>0) tim = m_Fram/Dura;
	m_Fram++;
	char str[128];	
	sprintf(str,"刷屏：%2d 帧/秒 %s",tim,test);
//
	dc.TextOut(50,50,str);  
//dc.SelectObject ( oldFont);  
}

//////////////////////////////////////////////////////////////////////////////
// PostRenderScene
// perform post display processing
// The default PostRenderScene places the framerate in the
// view's title. Replace this with your own title if you like.
void CApparatusTestView::PostRenderScene( void )
{
    // Only update the title every 15 redraws (this is about
    // every 1/2 second)
    static int updateFrame = 15;
    if (16 > ++updateFrame )
        return;
    updateFrame = 0;
    char string[256];
    _snprintf( string, 200, "%s ( %d Frames/sec )",
        (const char*)m_WindowTitle, FramesPerSecond() );
    GetParentFrame()->SetWindowText( string );
}
//////////////////////////////////////////////////////////////////////////////

// FramesPerSecond

// fetch frame rate calculations

int CApparatusTestView::FramesPerSecond( void )

{

    double eTime = ElapsedTimeinMSSinceLastRender();

    if ( 0 == (int)eTime )

        return 0;

    return (int)(1000/(int)eTime);

}

DWORD CApparatusTestView::ElapsedTimeinMSSinceLastStartup()

{

    return(m_ElapsedTime - m_StartTime);
	

}

DWORD CApparatusTestView::ElapsedTimeinMSSinceLastRender()

{

    return(m_ElapsedTime - m_previousElapsedTime);

}