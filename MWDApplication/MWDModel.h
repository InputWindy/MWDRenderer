#pragma once
#include <../assimp/Importer.hpp>
#include <../assimp/scene.h>
#include <../assimp/postprocess.h>
#include "MWDMesh.h"
#include "MWDMaterial.h"
#include "MWDTransform.h"
#include "MWDSkeleton.h"
#include "MWDAnimation.h"
using namespace math;

#define PrintVec3(_vec3)    \
cout<<"("<<_vec3.x<<","<<_vec3.y<<","<<_vec3.z<<")"<<endl;

//Model维护Mesh数组和一个材质
class MWDRenderer;
class MWDModel
{
private:
    map<string, int> bone_name_to_index_map;
public:
    void Log() {
        for (map<string,int>::iterator iter = bone_name_to_index_map.begin(); iter != bone_name_to_index_map.end(); ++iter) {
            cout << "(" << iter->first << "," << iter->second << ")" << endl;
        }
    }
	// constructor, expects a filepath to a 3D model.
	MWDModel(string name,string const& path, bool gamma = false) : gammaCorrection(gamma)
	{
        m_rootSkeleton = NULL;
        m_name = name;
		loadModel(path);
	}
	~MWDModel() {
        int num = m_meshes.size();
        for (int i = 0; i < num; ++i) {
            glDeleteVertexArrays(1,&(m_meshes[i].VAO));
            glDeleteBuffers(1, &(m_meshes[i].VBO));
            glDeleteBuffers(1, &(m_meshes[i].IBO));
        }
	}
public:
    //模型名称
    string                      m_name;
    
    MWDTransform                m_transform;

    //Mesh的顺序遍历方式
	vector<MWDMesh>		        m_meshes;

    //骨骼的顺序遍历方式
    vector<MWDSkeleton>         m_SkeletonNode;

    //骨骼的树形遍历方式
    MWDSkeleton*                m_rootSkeleton;

    //当前模型的所有动画
    vector<MWDAnimation>        m_animations;

	string directory;
	bool gammaCorrection;

    void Draw();
    void SetWorldPosition(float x, float y, float z) {
        m_transform.SetWorldPosition(x,y,z);
    }
    void SetWorldEulers(float yaw, float pitch, float roll) {
        m_transform.SetWorldEulers(yaw,pitch,roll);
    }
    void SetWorldScale(float x, float y, float z) {
        m_transform.SetWorldScale(x,y,z);
    }
    void TraverseSkeletonTree(MWDSkeleton* root_node) {
        if (!root_node) {
            return;
        }
        cout << root_node->m_skeletonName << endl;
        int child_num = root_node->m_skeletonChildren.size();
        for (int i = 0; i < child_num; ++i) {
            TraverseSkeletonTree(root_node->m_skeletonChildren[i]);
        }
    }
private:
    void loadModel(string const& path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // 错误提示
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));
        LoadAnimation(scene);                     //加载模型包含的所有动画
        LoadSkeleton(scene->mRootNode, scene);    //加载模型所有的骨骼
        BuildSkeletonTree(scene);                 //构造骨骼树
        LoadMesh(scene);                          //加载所有的蒙皮数据
        SetUpAllMeshes();                         //生成所有Mesh的VAO
    }
    //每个Node对应模型的一个子Mesh
    /*void processNode(aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            processMesh(node,mesh, scene);
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }*/
    
    private:
        //加载动画数据
        void LoadAnimation(const aiScene* scene) {
            if (scene->HasAnimations()) {
                cout << "loadModel:有动画！" << endl;
            };
        }
        //加载蒙皮数据
        void LoadMesh(const aiScene* scene) {
            if (scene->HasMeshes()) {
                int mesh_num = scene->mNumMeshes;
                for (int i = 0; i < mesh_num; ++i) {
                    processMesh(scene->mMeshes[i]);
                }
            }
        }
        //加载骨骼数据（递归）
        void LoadSkeleton(aiNode* node,const aiScene* scene) {
            MWDSkeleton ret = MWDSkeleton();
            int node_mesh_num = node->mNumMeshes;
            //遍历当前骨骼节点的所有蒙皮
            for (int i = 0; i < node_mesh_num; ++i) {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                int bone_num = mesh->mNumBones;
                int bone_id = 0;
                //遍历当前蒙皮的所有骨骼
                for (int j = 0; j < bone_num; ++j) {
                    aiBone* bone = mesh->mBones[j];
                    string bone_name = string(bone->mName.data);
                    bone_id = GetBoneID(bone_name);
                    //bone_id不是-1说明这个骨骼已经加载过了，直接跳过该骨骼
                    if (bone_id != -1) {
                        continue;
                    }
                    //加载一个新骨骼
                    else {
                        bone_name_to_index_map.insert(map<string,int>::value_type(bone_name, bone_name_to_index_map.size()));
                        MWDSkeleton skeleton = MWDSkeleton();
                        skeleton.m_Owner = this;
                        skeleton.m_pParent = NULL;
                        skeleton.m_skeletonName = bone_name;
                        skeleton.m_bone.m_bAllowRotation = true;
                        skeleton.m_bone.m_bAllowTranslation = true;
                        skeleton.m_bone.m_bIsEffector = true;
                        skeleton.m_bone.m_fIKWeight = 0;
                        skeleton.m_bone.m_fMaxRotation = 135.0f;
                        skeleton.m_bone.m_fMinRotation = 0.0f;
                        skeleton.m_bone.m_fMaxTranslation = 2.0f;
                        skeleton.m_bone.m_fMinTranslation = 0.0f;
                        for (int m = 0; m < 4; ++m) {
                            for (int n = 0; n < 4; n++)
                            {
                                skeleton.m_bone.m_OffSetMatrix[m][n] = bone->mOffsetMatrix[m][n];
                            }
                        }
                        skeleton.m_bone.m_TargetPosInWorld = vec3(0.0f);
                        skeleton.m_bone.m_TransformMatrix = mat4(1.0f);
                        m_SkeletonNode.push_back(skeleton);
                    }
                }
            }
            //递归遍历子节点
            int child_node_num = node->mNumChildren;
            for (int j = 0; j < child_node_num; ++j) {
                LoadSkeleton(node->mChildren[j],scene);
            }
        }
        void BuildSkeletonTree(const aiScene* scene) {
            #pragma region 手动生成根骨
            m_rootSkeleton = new MWDSkeleton();
            m_rootSkeleton->m_Owner = this;
            m_rootSkeleton->m_pParent = NULL;
            m_rootSkeleton->m_skeletonName = string("rootSkeleton");
            m_rootSkeleton->m_bone.m_bAllowRotation = false;
            m_rootSkeleton->m_bone.m_bAllowTranslation = false;
            m_rootSkeleton->m_bone.m_bIsEffector = false;
            m_rootSkeleton->m_bone.m_fIKWeight = 0.0f;
            m_rootSkeleton->m_bone.m_fMaxRotation = 0.0f;
            m_rootSkeleton->m_bone.m_fMinRotation = 0.0f;
            m_rootSkeleton->m_bone.m_fMaxTranslation = 0.0f;
            m_rootSkeleton->m_bone.m_fMinTranslation = 0.0f;
            m_rootSkeleton->m_bone.m_OffSetMatrix = mat4(1.0f);
            m_rootSkeleton->m_bone.m_TargetPosInWorld = this->m_transform.GetWorldPosition();
            m_rootSkeleton->m_bone.m_TransformMatrix = mat4(1.0f);
            #pragma endregion
            int child_num = scene->mRootNode->mNumChildren;
            for (int i = 0; i < child_num; ++i) {
                ProcessSkeleton(m_rootSkeleton, scene->mRootNode->mChildren[i]);
            }
        }
        //设置所有mesh的vao，vbo，ibo
        void SetUpAllMeshes() {
            for (int i = 0; i < m_meshes.size(); ++i) {
                m_meshes[i].setupMesh();
            }
        }
        //如果没找到，就返回-1
        int GetBoneID(string bone_name) {
            map<string,int>::iterator iter = bone_name_to_index_map.find(bone_name);
            if (iter != bone_name_to_index_map.end()) {
                return iter->second;
            }
            //cout << "找不到的名字：" << bone_name << "  " << -1 << endl;
            return -1;
        }
        //填写：名称（√），Owner（√），vertices（√），indices（√）
        void processMesh(aiMesh* mesh)
        {
            MWDMesh ret = MWDMesh();
            ret.m_meshName = string(mesh->mName.data);
            ret.m_Owner = this;

            //需要填写的数据
            vector<Vertex> vertices;
            vector<unsigned int> indices;

            // 解析vertices
            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;
                glm::vec3 vector;
                // positions
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.Position = vector;

                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;

                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;

                // normals
                if (mesh->HasNormals())
                {
                    vector.x = mesh->mNormals[i].x;
                    vector.y = mesh->mNormals[i].y;
                    vector.z = mesh->mNormals[i].z;
                    vertex.Normal = vector;
                }

                //bones
                if (mesh->HasBones()) {
                    int bone_id = 0; 
                    //i是当前vertex在当前mesh里的索引
                    for (int bone_idx = 0; bone_idx < mesh->mNumBones; ++bone_idx) {
                        bone_id = GetBoneID(string(mesh->mBones[bone_idx]->mName.data));
                        float bone_weight = 0;
                        for (int weight_id = 0; weight_id < mesh->mBones[bone_idx]->mNumWeights; ++weight_id) {
                            if (i == mesh->mBones[bone_idx]->mWeights[weight_id].mVertexId) {
                                bone_weight = mesh->mBones[bone_idx]->mWeights[weight_id].mWeight;
                                vertex.AddBoneData(bone_id, bone_weight);
                                //cout << "当前mesh的顶点ID：" << i << "  " << bone_weight << endl;
                                break;
                            }
                        }
                    }
                }

                // texture coordinates
                if (mesh->mTextureCoords[0])
                {
                    int uv_num = mesh->GetNumUVChannels();
                    glm::vec2 vec;
                    // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                    // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                    for (int j = 0; j < uv_num; ++j) {
                        vec.x = mesh->mTextureCoords[j][i].x;
                        vec.y = mesh->mTextureCoords[j][i].y;
                        vertex.TexCoords[j] = vec;
                    }
                }
                else {
                    for (int j = 0; j < 8; ++j) {
                        vertex.TexCoords[j] = glm::vec2(0.0f, 0.0f);
                    }
                }
                vertices.push_back(vertex);
            }

            //解析indices
            for (unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }
        
            ret.indices = indices;
            ret.vertices = vertices;

            m_meshes.push_back(ret);
            return ;
        }
        //连接骨骼的父子节点
        void ProcessSkeleton(MWDSkeleton* parent,aiNode* node) {
            cout << parent->m_skeletonName << endl;
            int skeleton_id = GetBoneID(string(node->mName.data));
            if (skeleton_id == -1) {
                string name(node->mName.data);
                cout << "没找到子骨骼"<<"  "<< name << endl;
                return;
            }
            MWDSkeleton* cur_skeleton = &m_SkeletonNode[skeleton_id];
            cur_skeleton->m_pParent = parent;
            parent->addChildSkeleton(cur_skeleton);
            int child_num = node->mNumChildren;
            for (int i = 0; i < child_num; ++i) {
                ProcessSkeleton(cur_skeleton, node->mChildren[i]);
            }
        }

};

