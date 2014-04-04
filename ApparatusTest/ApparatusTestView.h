
// ApparatusTestView.h : CApparatusTestView ��Ľӿ�
//

#pragma once
#include "CBMPLoader.h"
#include "Camera.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "3DSLoader.h"
#include "Vector.h"
#include "GLFont.h"


struct OBJPOSI
 {	float x,z;
	float h;
	float size;	
	int   cactus;
 };

class CApparatusTestView : public CView
{
protected: // �������л�����
	CApparatusTestView();
	DECLARE_DYNCREATE(CApparatusTestView)

// ����
public:
	CApparatusTestDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CApparatusTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	HGLRC		m_hRC;			//OpenGL����������
	CDC*	    m_pDC;			//OpenGL�豸������
	C3DSLoader m_3DS;                         // 3DS�ļ������� 
	CTerrain   m_Terrain;                     // ������ 
	CSkyBox    m_SkyBox;                      // ����� 
	Camera     m_Camera;                      // �������      
  	float      m_Fps;                         // ֡�� 
	Vector3    dirvec;
	UINT       g_cactus[5];                   //װ������
	OBJPOSI    objposi[TREESL];
	float      g_terrain  [MAP_W * MAP_W][3];
	//bool       temp;                      //�ֶ������Զ�
	
// ���ɵ���Ϣӳ�亯��
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
	//����
	CGLFont *Font;
	int m_Time,m_Fram,tim;
	HFONT hFont,hFont0,hFont1;    //����
	void text();   //��ʾ����
	void tree();
	void CreateFontFun();
	//
	//��ײ���
	void LoadT16(char *filename, GLuint &texture);  //������λͼ
	void ShowTree(int i);                            //չʾ��
	float GetHeight(float x,float z);                //��ø߶�
	void settml(int p);  
	void CollisionDetection(float x,float z);       //��ײ��⺯��

	//
	afx_msg void On32771();
	afx_msg void On32772();
	afx_msg void On32774();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void On32775();
};

#ifndef _DEBUG  // ApparatusTestView.cpp �еĵ��԰汾
inline CApparatusTestDoc* CApparatusTestView::GetDocument() const
   { return reinterpret_cast<CApparatusTestDoc*>(m_pDocument); }
#endif

