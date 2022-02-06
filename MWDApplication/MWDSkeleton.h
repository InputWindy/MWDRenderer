#pragma once
#include "headers.h"
#include "MWDBoneNode.h"
using namespace math;
class MWDModel;
//骨骼维护一个骨骼节点信息，一个aabb3包围盒，树形拓扑结构，骨骼名称
class MWDSkeleton
{
	friend class MWDModel;
public:
	//骨骼数据
	MWDBoneNode			m_bone;

	MWDSkeleton*		m_pParent;
	vector<MWDSkeleton*> m_skeletonChildren;

	//当前骨骼所属的模型
	MWDModel*			m_Owner;

	//骨骼名称
	string				m_skeletonName;
private:
	void addChildSkeleton(MWDSkeleton* skeleton) {
		m_skeletonChildren.push_back(skeleton);
	}
};

