
// ApparatusTestView.cpp : CApparatusTestView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "ApparatusTest.h"
#endif

#include "ApparatusTestDoc.h"
#include "ApparatusTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool temp = false;         //temp = true:       �����ֶ�����    ��ʼ��Ϊ�ر�
bool tempTimer = false;    //tempTimer = true:  �����Զ�����    ��ʼ��Ϊ�ر�
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
	ON_COMMAND(ID_32771, &CApparatusTestView::On32771)   //�ֶ�����
	ON_COMMAND(ID_32772, &CApparatusTestView::On32772)   //�Զ�����
	ON_COMMAND(ID_32774, &CApparatusTestView::On32774)   //��ͣ
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_32775, &CApparatusTestView::On32775)
END_MESSAGE_MAP()

// CApparatusTestView ����/����

CApparatusTestView::CApparatusTestView()
{
	// TODO: �ڴ˴���ӹ������
	
}

CApparatusTestView::~CApparatusTestView()
{
}

BOOL CApparatusTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	cs.style |=WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	/////////////////////////////////////////////
	return CView::PreCreateWindow(cs);
}

// CApparatusTestView ����

void CApparatusTestView::OnDraw(CDC* /*pDC*/)
{
	CApparatusTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
			
	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	RenderScene();	
	::SwapBuffers(m_pDC->GetSafeHdc());	
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


// CApparatusTestView ���

#ifdef _DEBUG
void CApparatusTestView::AssertValid() const
{
	CView::AssertValid();
}

void CApparatusTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CApparatusTestDoc* CApparatusTestView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CApparatusTestDoc)));
	return (CApparatusTestDoc*)m_pDocument;
}
#endif //_DEBUG



// CApparatusTestView ��Ϣ�������


bool CApparatusTestView::init(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);						
	glClearDepth(1.0f);											
	glDepthFunc(GL_LEQUAL);										
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);	
	glShadeModel(GL_SMOOTH);									
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	// ��ʼ������ 
	if(!m_Terrain.init())
	{
		
		::MessageBox(NULL,_T("��ʼ������ʧ��!"),_T("����"),MB_OK);
		exit(0);
	}

	// ��ʼ�����
	if(!m_SkyBox.init())
	{
		::MessageBox(NULL,_T("��ʼ�����ʧ��!"),_T("����"),MB_OK);
		exit(0);
	}


	// ��ʼ��3DS�ļ� 
	m_3DS.Init("model.3DS");

     //	m_Terrain.CustomizePath();
    // ��������� 
	m_Camera.setSpeed(2.0f);
	m_Camera.setCamera( 381, 35, 674,  374.5, 35, 669,  0, 1, 0);
	//��������
	CMenu * menu = new CMenu;
	menu->LoadMenuA(MAKEINTRESOURCE(IDD_DIALOG1));
	this->SetMenu(menu);
	
	//dirvec = m_Camera.getView() - m_Camera.getPosition();
	return true;                                        /**< �ɹ����� */
}


void CApparatusTestView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ	
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
	float y = m_Terrain.getAveHeight(x,z) ;  // ��ô˴����θ߶� 
	glTranslatef(x,y,z);
	glScalef(scale,scale,scale);
	m_3DS.Draw();                            // ��ʾ3DSģ�� 
	glPopMatrix();
}


void CApparatusTestView::RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
	glLoadIdentity();
	
	// ��������� 	
	m_Camera.setLook();
	
	// ��Ⱦ���� 
	m_Terrain.render();
	
	// ������� 
	m_SkyBox.render();

	// ��ʾ3DSģ�� 
	Show3DS(260,583,20);
		
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
	//test
	/*CDC * pDC = this->GetDC();	
	CString str;	
	int temp11 = m_Camera.getFps();
	str.Format("%d",temp11);
	pDC->TextOutA(100,130,str);
	this->ReleaseDC(pDC);*/
	//

	 ///�Զ����β��ִ���
	if(tempTimer == true)
	{
		m_Camera.ForwardOrBackward(m_Camera.getSpeed());   
		float angleY = 0.06f;	
	    m_Camera.rotate(angleY,0,1,0);
	 ///
	}
	CView::OnTimer(nIDEvent);
}






///�ֶ�����
void CApparatusTestView::On32771()
{
	// TODO: Add your command handler code here
	SetTimer(1,0.0001,NULL);
	temp = true;
	tempTimer = false;
}

///�Զ�����
void CApparatusTestView::On32772()
{
	// TODO: Add your command handler code here	
	//CView::OnTimer(1);
	temp = false;
	tempTimer = true;
	SetTimer(1,0.0001,NULL);
	m_Camera.ForwardOrBackward(m_Camera.getSpeed());  
	
}

///��ͣ����
void CApparatusTestView::On32774()
{
	// TODO: Add your command handler code here
	KillTimer(1);
}

///On_MouseUp����
void CApparatusTestView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//MessageBox(TEXT("nihao"));
	


	CView::OnLButtonUp(nFlags, point);
}

//��������
void CApparatusTestView::On32775()
{
	// TODO: Add your command handler code here
	CDialog * dlg = new CDialog;
	dlg->Create(MAKEINTRESOURCE(IDD_DIALOG1));
	dlg->ShowWindow(1);
}
