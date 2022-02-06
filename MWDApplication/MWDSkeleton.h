#pragma once
#include "headers.h"
#include "MWDBoneNode.h"
using namespace math;
class MWDModel;
//����ά��һ�������ڵ���Ϣ��һ��aabb3��Χ�У��������˽ṹ����������
class MWDSkeleton
{
public:
	//��������
	MWDBoneNode			m_bone;

	//ά���ӽڵ�任�����ڵ��T����
	mat4				m_TansformMatrix;

	MWDSkeleton*		m_pParent;
	vector<MWDSkeleton*> m_skeletonChildren;

	//��ǰ����������ģ��
	MWDModel*			m_Owner;

	//��������
	string				m_skeletonName;
private:
	void addSkeleton(MWDSkeleton* skeleton) {
		m_skeletonChildren.push_back(skeleton);
	}
};

