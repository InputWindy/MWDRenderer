#pragma once
#include "headers.h"
#include "MWDBoneNode.h"
using namespace math;
class MWDModel;
//骨骼维护一个骨骼节点信息，一个aabb3包围盒，树形拓扑结构，骨骼名称
class MWDSkeleton
{
public:
	//骨骼数据
	MWDBoneNode			m_bone;

	//维护子节点变换到父节点的T矩阵
	mat4				m_TansformMatrix;

	MWDSkeleton*		m_pParent;
	vector<MWDSkeleton*> m_skeletonChildren;

	//当前骨骼所属的模型
	MWDModel*			m_Owner;

	//骨骼名称
	string				m_skeletonName;
private:
	void addSkeleton(MWDSkeleton* skeleton) {
		m_skeletonChildren.push_back(skeleton);
	}
};

