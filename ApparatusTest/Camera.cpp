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

//  更新摄像机的位置,朝向和向上向量 
void Camera::setCamera( float positionX, float positionY, float positionZ,
				  		float viewX,     float viewY,     float viewZ,
						float upVectorX, float upVectorY, float upVectorZ)
{
    
	m_Position = Vector3(positionX, positionY, positionZ);	
	m_View     = Vector3(viewX, viewY, viewZ);	
	m_UpVector = Vector3(upVectorX, upVectorY, upVectorZ);

}

//  旋转摄像机(左、右、上、下)
void Camera::rotate(float angle, float x, float y, float z)//  x,y,z 是旋转轴方向向量的方向余弦  
{
	Vector3 newView,view = m_View - m_Position;		

	float cosTheta = (float)Fast_Cos(angle);
	float sinTheta = (float)Fast_Sin(angle);

	// 计算旋转向量的x值 
	// 公式： xs*(costheta+x*x*(1-costheta)) + ys*(x*y*(1-costheta)-z*sintheta) + zs*(x*z*(1-costheta)+y*sintheta) 
	newView.x  = (cosTheta + (1 - cosTheta) * x * x)		* view.x;
	newView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* view.y;
	newView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* view.z;

	// 计算旋转向量的y值 
    // 公式： ys*(costheta+y*y*(1-costheta)) + zs*(y*z*(1-costheta)-x*sintheta) + xs*(y*x*(1-costheta)+z*sintheta) 
	newView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* view.x;
	newView.y += (cosTheta + (1 - cosTheta) * y * y)		* view.y;
	newView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* view.z;

	// 计算旋转向量的z值 
	// 公式： z1*(costheta+z*z*(1-costheta)) + x1*(z*x*(1-costheta)-y*sintheta) + y1*(z*y*(1-costheta)+x*sintheta) 
	newView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* view.x;
	newView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* view.y;
	newView.z += (cosTheta + (1 - cosTheta) * z * z)		* view.z;

	// 更新摄像机的朝向
	m_View = m_Position + newView;
}




// 左右移动摄像机 
void Camera::LeftOrRight(float speed)
{
	Vector3 yaw;
	Vector3 oldPos,oldView;
	Vector3 cross = m_View - m_Position;
	oldPos = m_Position;
	oldView = m_View;
	cross = cross.crossProduct(m_UpVector);

	///归一化向量
	yaw = cross.normalize();
 
	m_Position.x += yaw.x * speed;
	m_Position.z += yaw.z * speed;

	m_View.x += yaw.x * speed;
	m_View.z += yaw.z * speed;
	
	// 进行边界检查和限定 
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

// 前后移动摄像机 
void Camera::ForwardOrBackward(float speed)
{

	Vector3 vector = m_View - m_Position;
	vector = vector.normalize();         //< 单位化 
	Vector3 oldPos,oldView;
	oldPos = m_Position;
	oldView = m_View;
	
	// 更新摄像机 
	m_Position.x += vector.x * speed;    // 根据速度更新位置 
	m_Position.z += vector.z * speed;	
	m_Position.y += vector.y * speed;
	
	
	m_View.x += vector.x * speed;		 // 根据速度更新方向 	
	m_View.y += vector.y * speed;
	m_View.z += vector.z * speed;
	
    // 进行边界检查和限定 	
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

// 布置摄像机
void Camera::setLook()
{
	

	gluLookAt(m_Position.x, m_Position.y, m_Position.z,	
			  m_View.x,	 m_View.y,     m_View.z,	
			  m_UpVector.x, m_UpVector.y, m_UpVector.z);
}

//计算帧速
int Camera::CaculateFrameRate()
{
    static int framesPerSecond = 0.0f;   //保存显示帧数
	 static float lastTime = 0.0f;    //记录上次时间
	float currentTime = GetTickCount() * 0.001f;    //获得当前时间秒
	++framesPerSecond;    //显示帧数递增1
	//如果时间差大于1秒
	if(currentTime - lastTime > 1.0f)
	{
		lastTime = currentTime;   //保存当前时间
		m_Fps = framesPerSecond;   //将当前帧数传给m_Fps
		framesPerSecond = 0;        //将帧数置零
	}
	return m_Fps;
}

//鼠标旋转摄像机
void Camera::setViewByMouse()
{
	POINT mousePos;     //保存当前鼠标位置
	int middleX = GetSystemMetrics(SM_CXSCREEN) >> 1;   //得到屏幕宽度的一半
	int middleY = GetSystemMetrics(SM_CYSCREEN) >> 1;   //得到屏幕高度的一半
	float angleY = 0.0f;    //摄像机左右旋转的角度
	float angleZ = 0.0f;    //摄像机上下旋转的角度
	static float currentRotX = 0.0f;
	//得到当前鼠标位置
	GetCursorPos(&mousePos);
	SetCursor(::LoadCursor(NULL,IDC_CROSS));
	ShowCursor(TRUE);
	//如果鼠标没有移动，则不用更新
	if((mousePos.x == middleX) && (mousePos.y == middleY))
		return;
	//设置鼠标位置在屏幕中心
	SetCursorPos(middleX,middleY);
	//得到鼠标移动方向
	angleY = (float)(middleX - mousePos.x)/1000.0f;
	angleZ = (float)(middleY - mousePos.y)/1000.0f;
	static float lastRotX = 0.0f;    //用于保存旋转角度
	lastRotX = currentRotX;
	//跟踪摄像机上下旋转角度
	currentRotX += angleZ;
	//如果上下旋转弧度大于1.0，我们截取到1.0并旋转
	if(currentRotX > 1.0f)
	{
		currentRotX = 1.0f;
		//根据保存的角度旋转方向
		if(lastRotX != 1.0f)
		{
			//通过叉积找到与旋转方向垂直的向量
			Vector3 vAxis = m_View - m_Position;
			vAxis = vAxis.crossProduct(m_UpVector);
			vAxis = vAxis.normalize();
			///旋转
			rotate(1.0f-lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	else if(currentRotX < -1.0f)
	{
		currentRotX = -1.0f;
		if(lastRotX != -1.0f)
		{
			//通过叉积找到与旋转方向垂直的向量
			Vector3 vAxis = m_View - m_Position;
			vAxis = vAxis.crossProduct(m_UpVector);
			vAxis = vAxis.normalize();
			///旋转
			rotate(1.0f-lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	//否则就选择angleZ度
	else 
	{
		//找到与旋转方向垂直的向量
		Vector3 vAxis = m_View - m_Position;
		vAxis = vAxis.crossProduct(m_UpVector);
		vAxis = vAxis.normalize();
		///旋转
		rotate(angleZ, vAxis.x, vAxis.y, vAxis.z);
	}
	rotate(angleY, 0, 1, 0);
}

//自动漫游
void Camera::AutoMove(float speed)
{
	CPoint point;
	GetCursorPos(&point);
}