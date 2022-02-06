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

//Modelά��Mesh�����һ������
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
    //ģ������
    string                      m_name;
    
    MWDTransform                m_transform;

    //Mesh��˳�������ʽ
	vector<MWDMesh>		        m_meshes;

    //������˳�������ʽ
    vector<MWDSkeleton>         m_SkeletonNode;

    //���������α�����ʽ
    MWDSkeleton*                m_rootSkeleton;

    //��ǰģ�͵����ж���
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
private:
    void loadModel(string const& path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // ������ʾ
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));
        LoadAnimation(scene);                     //����ģ�Ͱ��������ж���
        LoadSkeleton(-1,scene->mRootNode, scene); //����ģ�����еĹ�����ά�����ӹ�ϵ��
        LoadMesh(scene);                          //�������е���Ƥ���ݣ���Ƥ���ݲ�ά�����ӹ�ϵ��
        SetUpAllMeshes();                         //��������Mesh��VAO
    }
    //ÿ��Node��Ӧģ�͵�һ����Mesh
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
        //���ض�������
        void LoadAnimation(const aiScene* scene) {
            if (scene->HasAnimations()) {
                cout << "loadModel:�ж�����" << endl;
            };
        }
        //������Ƥ����
        void LoadMesh(const aiScene* scene) {
            if (scene->HasMeshes()) {
                int mesh_num = scene->mNumMeshes;
                for (int i = 0; i < mesh_num; ++i) {
                    processMesh(scene->mMeshes[i]);
                }
            }
        }
        //���ع������ݣ��ݹ飩
        void LoadSkeleton(int parent_skeleton, aiNode* node,const aiScene* scene) {
            MWDSkeleton ret = MWDSkeleton();
            int node_mesh_num = node->mNumMeshes;
            //������ǰ�����ڵ��������Ƥ
            for (int i = 0; i < node_mesh_num; ++i) {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                int bone_num = mesh->mNumBones;
                int bone_id = 0;
                //������ǰ��Ƥ�����й���
                for (int j = 0; j < bone_num; ++j) {
                    aiBone* bone = mesh->mBones[j];
                    string bone_name = string(bone->mName.data);
                    bone_id = GetBoneID(bone_name);
                    //bone_id����-1˵����������Ѿ����ع��ˣ�ֱ�������ù���
                    if (bone_id != -1) {
                        continue;
                    }
                    //����һ���¹���
                    else {
                        bone_name_to_index_map.insert(map<string,int>::value_type(bone_name, bone_name_to_index_map.size()));
                        //if (parent_skeleton == -1) {
                        //    //���ڵ�
                        //}
                        //else {
                        //    //�Ǹ��ڵ�
                        //}
                    }
                }
            }
            //�ݹ�����ӽڵ�
            int child_node_num = node->mNumChildren;
            for (int j = 0; j < child_node_num; ++j) {
                LoadSkeleton(-1,node->mChildren[j],scene);
            }
        }

        //��������mesh��vao��vbo��ibo
        void SetUpAllMeshes() {
            for (int i = 0; i < m_meshes.size(); ++i) {
                m_meshes[i].setupMesh();
            }
        }

        //���û�ҵ����ͷ���-1
        int GetBoneID(string bone_name) {
            map<string,int>::iterator iter = bone_name_to_index_map.find(bone_name);
            if (iter != bone_name_to_index_map.end()) {
                return iter->second;
            }
            //cout << "�Ҳ��������֣�" << bone_name << "  " << -1 << endl;
            return -1;
        }
        //��д�����ƣ��̣���Owner���̣���vertices��indices�����ڵ㣬�ӽڵ�
        void processMesh(aiMesh* mesh)
        {
            MWDMesh ret = MWDMesh();
            ret.m_meshName = string(mesh->mName.data);
            ret.m_Owner = this;

            //��Ҫ��д������
            vector<Vertex> vertices;
            vector<unsigned int> indices;

            // ����vertices
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
                    //i�ǵ�ǰvertex�ڵ�ǰmesh�������
                    for (int bone_idx = 0; bone_idx < mesh->mNumBones; ++bone_idx) {
                        bone_id = GetBoneID(string(mesh->mBones[bone_idx]->mName.data));
                        float bone_weight = 0;
                        for (int weight_id = 0; weight_id < mesh->mBones[bone_idx]->mNumWeights; ++weight_id) {
                            if (i == mesh->mBones[bone_idx]->mWeights[weight_id].mVertexId) {
                                bone_weight = mesh->mBones[bone_idx]->mWeights[weight_id].mWeight;
                                vertex.AddBoneData(bone_id, bone_weight);
                                //cout << "��ǰmesh�Ķ���ID��" << i << "  " << bone_weight << endl;
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

            //����indices
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

};

