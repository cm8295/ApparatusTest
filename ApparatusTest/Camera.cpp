#include "stdafx.h"
#include "Camera.h"                   
#include "Vector.h"                  
#include "UtilityFunc.h"


Camera* Camera::m_pCamera = NULL;


Camera::Camera()
{
	m_Position	= Vector3(0.0, 0.0, 0.0);					
	m_View		= Vector3(0.0, 1.0, 0.5);					
	m_UpVector	= Vector3(0.0, 0.0, 1.0);
	m_Speed     = 0.1f;
	m_pCamera   = this;
	Build_Sin_Cos_Tables();
}


Camera::~Camera()
{
}

//  �����������λ��,������������� 
void Camera::setCamera( float positionX, float positionY, float positionZ,
				  		float viewX,     float viewY,     float viewZ,
						float upVectorX, float upVectorY, float upVectorZ)
{
    
	m_Position = Vector3(positionX, positionY, positionZ);	
	m_View     = Vector3(viewX, viewY, viewZ);	
	m_UpVector = Vector3(upVectorX, upVectorY, upVectorZ);

}

//  ��ת�����(���ҡ��ϡ���)
void Camera::rotate(float angle, float x, float y, float z)//  x,y,z ����ת�᷽�������ķ�������  
{
	Vector3 newView,view = m_View - m_Position;		

	float cosTheta = (float)Fast_Cos(angle);
	float sinTheta = (float)Fast_Sin(angle);

	// ������ת������xֵ 
	// ��ʽ�� xs*(costheta+x*x*(1-costheta)) + ys*(x*y*(1-costheta)-z*sintheta) + zs*(x*z*(1-costheta)+y*sintheta) 
	newView.x  = (cosTheta + (1 - cosTheta) * x * x)		* view.x;
	newView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* view.y;
	newView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* view.z;

	// ������ת������yֵ 
    // ��ʽ�� ys*(costheta+y*y*(1-costheta)) + zs*(y*z*(1-costheta)-x*sintheta) + xs*(y*x*(1-costheta)+z*sintheta) 
	newView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* view.x;
	newView.y += (cosTheta + (1 - cosTheta) * y * y)		* view.y;
	newView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* view.z;

	// ������ת������zֵ 
	// ��ʽ�� z1*(costheta+z*z*(1-costheta)) + x1*(z*x*(1-costheta)-y*sintheta) + y1*(z*y*(1-costheta)+x*sintheta) 
	newView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* view.x;
	newView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* view.y;
	newView.z += (cosTheta + (1 - cosTheta) * z * z)		* view.z;

	// ����������ĳ���
	m_View = m_Position + newView;
}




// �����ƶ������ 
void Camera::LeftOrRight(float speed)
{
	Vector3 yaw;
	Vector3 oldPos,oldView;
	Vector3 cross = m_View - m_Position;
	oldPos = m_Position;
	oldView = m_View;
	cross = cross.crossProduct(m_UpVector);

	///��һ������
	yaw = cross.normalize();
 
	m_Position.x += yaw.x * speed;
	m_Position.z += yaw.z * speed;

	m_View.x += yaw.x * speed;
	m_View.z += yaw.z * speed;
	
	// ���б߽�����޶� 
	if(m_View.x > MAP_WIDTH - 20 || m_View.x < 2*CELL_WIDTH)
	{
		m_Position.x = oldPos.x ;
		m_View.x = oldView.x;
	}

	if(m_View.z > MAP_WIDTH - 20 || m_View.z < 2*CELL_WIDTH)
	{
		m_Position.z = oldPos.z ;
		m_View.z = oldView.z;
	}
	

}

// ǰ���ƶ������ 
void Camera::ForwardOrBackward(float speed)
{

	Vector3 vector = m_View - m_Position;
	vector = vector.normalize();         //< ��λ�� 
	Vector3 oldPos,oldView;
	oldPos = m_Position;
	oldView = m_View;
	
	// ��������� 
	m_Position.x += vector.x * speed;    // �����ٶȸ���λ�� 
	m_Position.z += vector.z * speed;	
	m_Position.y += vector.y * speed;
	
	
	m_View.x += vector.x * speed;		 // �����ٶȸ��·��� 	
	m_View.y += vector.y * speed;
	m_View.z += vector.z * speed;
	
    // ���б߽�����޶� 	
	if(m_View.x > MAP_WIDTH - 20 || m_View.x <  2*CELL_WIDTH)
	{
		m_Position.x = oldPos.x ;
		m_View.x = oldView.x;
	}

	if(m_View.z > MAP_WIDTH - 20 || m_View.z < 2*CELL_WIDTH)
	{
		m_Position.z = oldPos.z ;
		m_View.z = oldView.z;
	}
	
	
}

// ���������
void Camera::setLook()
{
	

	gluLookAt(m_Position.x, m_Position.y, m_Position.z,	
			  m_View.x,	 m_View.y,     m_View.z,	
			  m_UpVector.x, m_UpVector.y, m_UpVector.z);
}

//����֡��
int Camera::CaculateFrameRate()
{
    static int framesPerSecond = 0.0f;   //������ʾ֡��
	 static float lastTime = 0.0f;    //��¼�ϴ�ʱ��
	float currentTime = GetTickCount() * 0.001f;    //��õ�ǰʱ����
	++framesPerSecond;    //��ʾ֡������1
	//���ʱ������1��
	if(currentTime - lastTime > 1.0f)
	{
		lastTime = currentTime;   //���浱ǰʱ��
		m_Fps = framesPerSecond;   //����ǰ֡������m_Fps
		framesPerSecond = 0;        //��֡������
	}
	return m_Fps;
}

//�����ת�����
void Camera::setViewByMouse()
{
	POINT mousePos;     //���浱ǰ���λ��
	int middleX = GetSystemMetrics(SM_CXSCREEN) >> 1;   //�õ���Ļ��ȵ�һ��
	int middleY = GetSystemMetrics(SM_CYSCREEN) >> 1;   //�õ���Ļ�߶ȵ�һ��
	float angleY = 0.0f;    //�����������ת�ĽǶ�
	float angleZ = 0.0f;    //�����������ת�ĽǶ�
	static float currentRotX = 0.0f;
	//�õ���ǰ���λ��
	GetCursorPos(&mousePos);
	SetCursor(::LoadCursor(NULL,IDC_CROSS));
	ShowCursor(TRUE);
	//������û���ƶ������ø���
	if((mousePos.x == middleX) && (mousePos.y == middleY))
		return;
	//�������λ������Ļ����
	SetCursorPos(middleX,middleY);
	//�õ�����ƶ�����
	angleY = (float)(middleX - mousePos.x)/1000.0f;
	angleZ = (float)(middleY - mousePos.y)/1000.0f;
	static float lastRotX = 0.0f;    //���ڱ�����ת�Ƕ�
	lastRotX = currentRotX;
	//���������������ת�Ƕ�
	currentRotX += angleZ;
	//���������ת���ȴ���1.0�����ǽ�ȡ��1.0����ת
	if(currentRotX > 1.0f)
	{
		currentRotX = 1.0f;
		//���ݱ���ĽǶ���ת����
		if(lastRotX != 1.0f)
		{
			//ͨ������ҵ�����ת����ֱ������
			Vector3 vAxis = m_View - m_Position;
			vAxis = vAxis.crossProduct(m_UpVector);
			vAxis = vAxis.normalize();
			///��ת
			rotate(1.0f-lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	else if(currentRotX < -1.0f)
	{
		currentRotX = -1.0f;
		if(lastRotX != -1.0f)
		{
			//ͨ������ҵ�����ת����ֱ������
			Vector3 vAxis = m_View - m_Position;
			vAxis = vAxis.crossProduct(m_UpVector);
			vAxis = vAxis.normalize();
			///��ת
			rotate(1.0f-lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	//�����ѡ��angleZ��
	else 
	{
		//�ҵ�����ת����ֱ������
		Vector3 vAxis = m_View - m_Position;
		vAxis = vAxis.crossProduct(m_UpVector);
		vAxis = vAxis.normalize();
		///��ת
		rotate(angleZ, vAxis.x, vAxis.y, vAxis.z);
	}
	rotate(angleY, 0, 1, 0);
}

//�Զ�����
void Camera::AutoMove(float speed)
{
	CPoint point;
	GetCursorPos(&point);
}