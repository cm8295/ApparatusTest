#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "stdafx.h"
#include "Vector.h"               


// ������� 
class Camera
{
public:
	
	// ���캯������������ 
	Camera();
	~Camera();
	
	// ��������״̬���� 
	Vector3 getPosition()   {	return m_Position;		}
	Vector3 getView()	    {	return m_View;			}
	Vector3 getUpVector()   {	return m_UpVector;		}
	float   getSpeed()      {   return m_Speed;         }
     int     getFps()   {   return m_Fps;           }    //���֡��

	void    setPosition(float xx, float yy, float zz)
	{ m_Position.x = xx; m_Position.y = yy; m_Position.z = zz; }
	// �����ٶ� 
	void setSpeed(float speed)
	{ 
		m_Speed  = speed;
	}
     
	// �����������λ��, �۲����������� 
	void setCamera(float positionX, float positionY, float positionZ,
			 	   float viewX,     float viewY,     float viewZ,
				   float upVectorX, float upVectorY, float upVectorZ);

	// ��ת��������� 
	void rotate(float angle, float X, float Y, float Z);

	//�����������������۲췽��
	void setViewByMouse(); 
	
    // ����������ƶ� 
	void LeftOrRight(float speed);

	// ǰ���ƶ������ 
	void ForwardOrBackward(float speed);

	//�Զ������ƶ������
	void AutoMove(float speed);
	
	// ��������� 
	void setLook();

	//����֡��
	int CaculateFrameRate();

    //�õ������ָ��
	static Camera* GetCamera(void) { return m_pCamera;}


private:
	// ��������� 
	static Camera  *m_pCamera;      // ��ǰȫ�������ָ�� 
	Vector3        m_Position;      // λ�� 
	Vector3        m_View;          // ���� 
	Vector3        m_UpVector;      // �������� 
	float          m_Speed;         // �ٶ� 
     int     m_Fps;            //֡��

};

#endif //__CAMERA_H__
