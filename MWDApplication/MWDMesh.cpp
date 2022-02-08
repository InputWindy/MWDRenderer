#include "MWDMesh.h"
#include "MWDRenderer.h"
#include "MWDMaterial.h"
#include "MWDModel.h"
MWDMesh::MWDMesh()
{
	m_Owner = NULL;
	m_material = MWDMaterial::GetDefault();
}

//ÉèÖÃRenderCtxÈ»ºóDrawFrame()
void MWDMesh::DrawMesh(MWDRenderer* pRenderer)
{
	pRenderer->ms_Ctx.m_curMaterial = m_material;
	pRenderer->ms_Ctx.m_curMesh = this;

	int bone_num = m_Owner->m_SkeletonNode.size();
	for (int i = 0; i < bone_num; ++i) {
		pRenderer->ms_Ctx.m_boneTransform.push_back(m_Owner->m_SkeletonNode[i].m_bone.m_FinalTransformMatrix);
	}
	pRenderer->ms_Ctx.m_modelMatrix = m_Owner->m_transform.GetModelMatrix();

	pRenderer->DrawFrame();
}
