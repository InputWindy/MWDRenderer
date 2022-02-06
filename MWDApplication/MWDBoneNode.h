#pragma once
#include "MWDMath.h"

//维护一根骨骼的相关属性
class MWDBoneNode
{
public:
	MWDBoneNode() {
		
	}
	~MWDBoneNode() {

	}
public:
	//Use for IK
	bool m_bIsEffector;					//当前骨骼是否是效应器
	vec3 m_TargetPosInWorld;			//当前骨骼的世界空间坐标

	bool m_bAllowTranslation;	//是否允许平移变换
	float m_fMinTranslation;	//最小平移变换限度
	float m_fMaxTranslation;	//最大平移变换限度

	bool  m_bAllowRotation;		//是否允许旋转变换
	float m_fMinRotation;		//最小旋转变换限度
	float m_fMaxRotation;		//最大旋转变换限度

	float m_fIKWeight;					//IK权重

	mat4 m_OffSetMatrix;				//将顶点从T_Pose模型空间转换到骨骼空间(永远不变)
	mat4 m_TransformMatrix;				//将顶点从直接连接的骨骼一口气转换到根骨空间（实时更新）
};

