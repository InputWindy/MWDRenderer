#pragma once
#include "MWDShader.h"
#include "MWDUniform.h"
#include "MWDRenderState.h"
#include "MWDPass.h"
//Material维护ShaderProgram的Uniform变量
class MWDMaterial
{
protected:
    static inline MWDMaterial* m_screenTexture = NULL;
public:
    MWDMaterial() {
        m_shaderProgram = MWDShader::GetDefault();
    }
    MWDMaterial(string vshader,string fshader) {
        m_shaderProgram = new MWDShader(vshader.c_str(),fshader.c_str());
        *m_shaderProgram = *MWDShader::GetDefault();
    }
public:
    //初始化材质的时候调用AddUniform
    bool AddUniform(MWDUniform* uniform) {
        if (!uniform) {
            return false;
        }
        for (vector<MWDUniform*>::iterator iter = m_uniform.begin(); iter < m_uniform.end(); ++iter) {
            MWDUniform* tmp = *iter;
            if (tmp==uniform) {
                return false;
            }
        }
        m_uniform.push_back(uniform);
        return true;
    }
    //在编辑器中调用SetUniform来实时修改材质的参数
    template<typename T,typename N>
    bool SetUniform(string nameineditor,T data) {
        for (vector<MWDUniform*>::iterator iter = m_uniform.begin(); iter < m_uniform.end(); ++iter) {
            MWDUniform* tmp = *iter;
            if (tmp->nameInEditor == nameineditor) {
                ((N*)tmp)->m_data = data;
                return true;
            }
        }
        return false;
    }

    //初始化材质的时候调用AddSampler
    bool AddSampler(MWDTexture*texture) {
        if (!texture) {
            return false;
        }
        for (vector<MWDTexture*>::iterator iter = m_sampler.begin(); iter < m_sampler.end(); ++iter) {
            MWDTexture* tmp = *iter;
            if (tmp == texture) {
                return false;
            }
        }
        m_sampler.push_back(texture);
        return true;
    }
    bool SetSampler(string nameineditor,string path) {
        for (vector<MWDTexture*>::iterator iter = m_sampler.begin(); iter < m_sampler.end(); ++iter) {
            MWDTexture* tmp = *iter;
            if (tmp->nameInEditor == nameineditor) {
                *tmp = *new MWDTexture(nameineditor, tmp->m_tex_unit, path);;
                return true;
            }
        }
        return false;
    }
    //获取一个默认材质的实例(new了新空间)
    static MWDMaterial* GetDefault() {
        MWDMaterial* defaultMaterial = new MWDMaterial();
        if (defaultMaterial) {

            #pragma region 内置变量
            vec3 lightColor = vec3();
            MWDVec3* _lightColor = new MWDVec3(string("lightColor"), string("lightColor"), lightColor);
            defaultMaterial->AddUniform(_lightColor);

            vec3 lightpos = vec3();
            MWDVec3* _lightPos = new MWDVec3(string("lightPos"),string("lightPos"),lightpos);
            defaultMaterial->AddUniform(_lightPos);

            vec3 viewpos = vec3();
            MWDVec3* _viewpos = new MWDVec3(string("viewPos"), string("viewPos"), viewpos);
            defaultMaterial->AddUniform(_viewpos);

            mat4 model = mat4();
            MWDMat4* _model = new MWDMat4(string("model"),string("model_matrix"),model);
            defaultMaterial->AddUniform(_model);

            mat4 view = mat4();
            MWDMat4* _view = new MWDMat4(string("view"), string("view_matrix"), view);
            defaultMaterial->AddUniform(_view);

            mat4 proj = mat4();
            MWDMat4* _proj = new MWDMat4(string("projection"), string("proj_matrix"), proj);
            defaultMaterial->AddUniform(_proj);

            mat4 gWVP = mat4();
            MWDMat4* _gWVP = new MWDMat4(string("gWVP"), string("gWVP_matrix"), gWVP);
            defaultMaterial->AddUniform(_gWVP);
            #pragma endregion

            #pragma region 编辑器内设置的变量
            MWDTexture* tex = new MWDTexture(string("diffuse_map"), 0);
            defaultMaterial->AddSampler(tex);

            MWDTexture* tex1 = new MWDTexture(string("normal_map"), 1);
            defaultMaterial->AddSampler(tex1);

            MWDTexture* tex2 = new MWDTexture(string("roughness_map"), 2);
            defaultMaterial->AddSampler(tex2);

            MWDTexture* tex3 = new MWDTexture(string("metallic_map"), 3);
            defaultMaterial->AddSampler(tex3);

            MWDTexture* tex4 = new MWDTexture(string("ao_map"), 4);
            defaultMaterial->AddSampler(tex4);

            #pragma endregion

        }
        return defaultMaterial;
    }
    //new一个新材质
    static MWDMaterial* GetScreenMaterial() {
        if (!m_screenTexture) {
            m_screenTexture = new MWDMaterial("C:/Users/InputWindy/Desktop/MWDRenderer/shaders/offScreen.vert", "C:/Users/InputWindy/Desktop/MWDRenderer/shaders/offScreen.frag");
            MWDTexture* tex5 = new MWDTexture(string("screenTexture"), 0);
            m_screenTexture->AddSampler(tex5);
        }
        return m_screenTexture;
    }
public:
    MWDShader* m_shaderProgram;
    MWDRenderState m_renderState;
    vector<MWDUniform*> m_uniform;
    vector<MWDTexture*> m_sampler;
};

