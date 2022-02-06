#include "MWDModel.h"
#include "MWDRenderer.h"
void MWDModel::Draw()
{
    for (int i = 0; i < m_meshes.size(); ++i) {
        m_meshes[i].DrawMesh(MWDRenderer::GetMainRenderer());
    }
}
