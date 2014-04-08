
// ApparatusTestView.h : CApparatusTestView 类的接口
//

#pragma once
#include "CBMPLoader.h"
#include "Camera.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "3DSLoader.h"
#include "Vector.h"



class CApparatusTestView : public CView
{
protected: // 仅从序列化创建
	CApparatusTestView();
	DECLARE_DYNCREATE(CApparatusTestView)

// 特性
public:
	CApparatusTestDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CApparatusTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	HGLRC		m_hRC;			//OpenGL绘制描述表
	CDC*	    m_pDC;			//OpenGL设备描述表
	HDC         m_hDc;
	C3DSLoader m_3DS;                         // 3DS文件载入类 
	CTerrain   m_Terrain;                     // 地形类 
	CSkyBox    m_SkyBox;                      // 天空类 
	Camera     m_Camera;                      // 摄像机类      
  	float      m_Fps;                         // 帧速 

	Vector3    dirvec;
	UINT       g_cactus[5];                   //装载纹理
	float      g_terrain  [MAP_W * MAP_W][3];
	GLuint		g_index		[MAP_W*MAP_W* 2];
	
	DWORD m_StartTime, m_ElapsedTime, m_previousElapsedTime;
	CString m_WindowTitle;    //Window Title
// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	bool init(void);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void Show3DS(float x, float z, float scale);
	void RenderScene(void);
	bool SetupPixelFormat(void);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	bool InitializeOpenGL(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//字体
	int m_Time,m_Fram,tim;
	//显示帧速
	void CreateFontFun();       //  没用
	void PostRenderScene();     //2
	int FramesPerSecond();
	DWORD ElapsedTimeinMSSinceLastStartup();
	DWORD ElapsedTimeinMSSinceLastRender();
	//

	//
	afx_msg void On32771();
	afx_msg void On32772();
	afx_msg void On32774();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void On32775();
};

#ifndef _DEBUG  // ApparatusTestView.cpp 中的调试版本
inline CApparatusTestDoc* CApparatusTestView::GetDocument() const
   { return reinterpret_cast<CApparatusTestDoc*>(m_pDocument); }
#endif

