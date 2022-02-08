#pragma once
#include "MWDMath.h"

//维护一根骨骼的相关属性
class MWDBoneNode
{
public:
	MWDBoneNode() {
		m_FinalTransformMatrix = identity<mat4>();
	}
	~MWDBoneNode() {

	}
public:
	//Use for IK
	bool m_bIsEffector;					//当前骨骼是否是效应器
	
	bool m_bAllowTranslation;	//是否允许平移变换
	float m_fMinTranslation;	//最小平移变换限度
	float m_fMaxTranslation;	//最大平移变换限度

	bool  m_bAllowRotation;		//是否允许旋转变换
	float m_fMinRotation;		//最小旋转变换限度
	float m_fMaxRotation;		//最大旋转变换限度

	float m_fIKWeight;					//IK权重

	mat4 m_OffSetMatrix;				//将顶点从模型空间转换到骨骼空间(永远不变)
	mat4 m_TransformMatrix;				//变换到父骨骼的变换矩阵（由动画矩阵实时修改）
	mat4 m_TransformMatrixInverse;		//父骨骼变换到当前骨骼的变换矩阵（由动画矩阵实时修改）
	mat4 m_FinalTransformMatrix;		//当前顶点变换到根骨的矩阵（实时更新）
	vec3 m_TargetPosInWorld;			//当前骨骼的世界空间坐标（实时更新）


};

