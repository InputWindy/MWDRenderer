#pragma once
#include "MWDMath.h"

//ά��һ���������������
class MWDBoneNode
{
public:
	MWDBoneNode() {
		
	}
	~MWDBoneNode() {

	}
public:
	//Use for IK
	bool m_bIsEffector;					//��ǰ�����Ƿ���ЧӦ��
	vec3 m_TargetPosInWorld;			//��ǰ����������ռ�����

	bool m_bAllowTranslation;	//�Ƿ�����ƽ�Ʊ任
	float m_fMinTranslation;	//��Сƽ�Ʊ任�޶�
	float m_fMaxTranslation;	//���ƽ�Ʊ任�޶�

	bool  m_bAllowRotation;		//�Ƿ�������ת�任
	float m_fMinRotation;		//��С��ת�任�޶�
	float m_fMaxRotation;		//�����ת�任�޶�

	float m_fIKWeight;					//IKȨ��

	mat4 m_OffSetMatrix;				//�������T_Poseģ�Ϳռ�ת���������ռ�(��Զ����)
	mat4 m_TransformMatrix;				//�������ֱ�����ӵĹ���һ����ת�������ǿռ䣨ʵʱ���£�
};

