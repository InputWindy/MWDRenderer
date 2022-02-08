#pragma once
#include "MWDMath.h"

//ά��һ���������������
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
	bool m_bIsEffector;					//��ǰ�����Ƿ���ЧӦ��
	
	bool m_bAllowTranslation;	//�Ƿ�����ƽ�Ʊ任
	float m_fMinTranslation;	//��Сƽ�Ʊ任�޶�
	float m_fMaxTranslation;	//���ƽ�Ʊ任�޶�

	bool  m_bAllowRotation;		//�Ƿ�������ת�任
	float m_fMinRotation;		//��С��ת�任�޶�
	float m_fMaxRotation;		//�����ת�任�޶�

	float m_fIKWeight;					//IKȨ��

	mat4 m_OffSetMatrix;				//�������ģ�Ϳռ�ת���������ռ�(��Զ����)
	mat4 m_TransformMatrix;				//�任���������ı任�����ɶ�������ʵʱ�޸ģ�
	mat4 m_TransformMatrixInverse;		//�������任����ǰ�����ı任�����ɶ�������ʵʱ�޸ģ�
	mat4 m_FinalTransformMatrix;		//��ǰ����任�����ǵľ���ʵʱ���£�
	vec3 m_TargetPosInWorld;			//��ǰ����������ռ����꣨ʵʱ���£�


};

