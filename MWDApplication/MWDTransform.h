#pragma once
#include "headers.h"
class MWDTransform
{
	friend class MWDRenderer;
private:
	//世界坐标系三个轴(这个是固定的,实例化时填写)
	vec3 m_WorldUp;							//y(0,1,0)
	vec3 m_WorldRight;						//x(1,0,0)
	vec3 m_WorldFront;						//z(0,0,1)
	
	//本地坐标系三个轴（用于计算Rotation，用欧拉角计算）
	vec3 m_LocalUp;
	vec3 m_LocalRight;
	vec3 m_LocalFront;

	mat4 m_WorldRotation;		//旋转矩阵（状态矩阵）

protected:
	//世界坐标系（在editor中实时修改的值）
	vec3 m_WorldPosition;		//世界坐标
	vec3 m_WorldEuler;			//yaw，pitch，roll
	vec3 m_WorldScale;			//三方向的缩放
	
public:
	MWDTransform() {
		m_WorldUp = vec3(0.0f, 1.0f, 0.0f);
		m_WorldRight = vec3(1.0f, 0.0f, 0.0f);
		m_WorldFront = vec3(0.0f, 0.0f, 1.0f);

		m_LocalUp = vec3(1.0f);
		m_LocalRight = vec3(1.0f);
		m_LocalFront = vec3(1.0f);
		m_WorldRotation = mat4(1.0f);

		m_WorldPosition = vec3(0.0f, 0.0f, 0.0f);
		m_WorldEuler = vec3(0.0f, 0.0f, 0.0f);
		m_WorldScale = vec3(0.3f,0.3f,0.3f);
	}
	~MWDTransform() {

	}
	void SetWorldPosition(float x,float y,float z) {
		m_WorldPosition.x = x;
		m_WorldPosition.y = y;
		m_WorldPosition.z = z;
		update();
	}
	void SetWorldEulers(float yaw,float pitch,float roll) {
		m_WorldEuler.x = pitch;
		m_WorldEuler.y = yaw;
		m_WorldEuler.z = roll;
		update();
	}
	void SetWorldScale(float x,float y,float z) {
		m_WorldScale.x = x;
		m_WorldScale.y = y;
		m_WorldScale.z = z;
		update();
	}

	vec3 GetWorldPosition() {
		return m_WorldPosition;
	}
	vec3 GetWorldScale() {
		return m_WorldScale;
	}
	vec3 GetWorldEulers() {
		return m_WorldEuler;
	}

	mat4 GetModelMatrix() {
		mat4 _model= m_WorldRotation;
		_model = translate(_model, m_WorldPosition);
		_model = scale(_model, m_WorldScale);
		return _model;
	}
private:
	void update() {
		mat4 rotate_with_x = mat4(1.0f);
		mat4 rotate_with_y = mat4(1.0f);
		mat4 rotate_with_z = mat4(1.0f);

		rotate_with_x = rotate(rotate_with_x,radians(m_WorldEuler.x),m_WorldRight);
		rotate_with_y = rotate(rotate_with_y, radians(m_WorldEuler.y), m_WorldUp);
		rotate_with_z = rotate(rotate_with_z, radians(m_WorldEuler.z), m_WorldFront);

		m_LocalUp = mat3(rotate_with_x) * m_WorldUp;
		m_LocalRight = mat3(rotate_with_x) * m_WorldRight;
		m_LocalFront = mat3(rotate_with_x) * m_WorldFront;

		m_WorldRotation = m_WorldRotation * rotate_with_x * rotate_with_y * rotate_with_z;
	}

};

