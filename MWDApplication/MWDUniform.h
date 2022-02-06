#pragma once
#include "headers.h"

//纹理单元是每次DrawCall固定的公共资源，Texture不是公共资源，所在的texture unit也是可以随时变化的
//使用方法：先bind，再SetSampler，再Unbind
class MWDTexture {
public:
    //Shader贴图导入
    MWDTexture(string nameineditor, unsigned int tex_unit, string path = string("C:/Users/InputWindy/Desktop/MyRenderer/MWDEngine/img/default.jpeg")) {
        m_path = path;
        m_tex_unit = tex_unit;
        nameInEditor = nameineditor;
        id = loadTexture(path.c_str(), tex_unit);
    }
    ~MWDTexture() {
        glDeleteTextures(1,&id);
    }
    void Bind() {
        glActiveTexture(GL_TEXTURE0 + m_tex_unit);
        glBindTexture(GL_TEXTURE_2D, id);
    }
    void UnBind() {
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void operator=(const MWDTexture& texture) {
        m_path = texture.m_path;
        nameInEditor = texture.nameInEditor;
        m_tex_unit = texture.m_tex_unit;
        width = texture.width;
        height = texture.height;
        nrComponents = texture.nrComponents;
        glDeleteTextures(1, &id);
        id = texture.id;
    }
public:
    unsigned int id;
    unsigned int m_tex_unit;
    int width;
    int height;
    int nrComponents;
    string m_path;
    string nameInEditor;
private:
    //从文件读取一张texture2D(自己设定纹理单元)：一次DrawCall想同时用多张纹理，必须让纹理在不同纹理单元里
    unsigned int loadTexture(char const* path, unsigned int tex_unit)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
        
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;
            glActiveTexture(tex_unit);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(0);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }
};


//维护Shader变量:名称
class MWDUniform
{
public:
    enum UT {
        INT,
        FLOAT,
        VEC2,
        VEC3,
        VEC4,
        MAT2,
        MAT3,
        MAT4,
        UT_MAX
    };
protected:
    MWDUniform(string nameinshader, string nameineditor) {
        nameInShader = nameinshader;
        nameInEditor = nameineditor;
    }
public:
    ~MWDUniform() {}
    virtual unsigned int GetUniformType() {
        return UT_MAX;
    }
    string nameInShader;
    string nameInEditor;
};

class MWDInt : public MWDUniform {
public:
    MWDInt(string nameinshader, string nameineditor, int data)
        :MWDUniform(nameinshader, nameineditor) {
        m_data = data;
    }
public:
    virtual unsigned int GetUniformType() {
        return INT;
    }
    int m_data;
};
class MWDFloat : public MWDUniform {
public:
    MWDFloat(string nameinshader, string nameineditor, float data)
        :MWDUniform(nameinshader, nameineditor) {
        m_data = data;
    }
public:
    virtual unsigned int GetUniformType() {
        return FLOAT;
    }
    float m_data;
};
class MWDVec2 : public MWDUniform {
public:
    MWDVec2(string nameinshader, string nameineditor, vec2 data)
        :MWDUniform(nameinshader, nameineditor) {
        m_data = data;
    }
public:
    virtual unsigned int GetUniformType() {
        return VEC2;
    }
    vec2 m_data;
};
class MWDVec3 : public MWDUniform {
public:
    MWDVec3(string nameinshader, string nameineditor, vec3 data)
        :MWDUniform(nameinshader, nameineditor) {
        m_data = data;
    }
public:
    virtual unsigned int GetUniformType() {
        return VEC3;
    }
    vec3 m_data;
};
class MWDVec4 : public MWDUniform {
public:
    MWDVec4(string nameinshader, string nameineditor, vec4 data)
        :MWDUniform(nameinshader, nameineditor) {
        m_data = data;
    }
public:
    virtual unsigned int GetUniformType() {
        return VEC4;
    }
    vec4 m_data;
};
class MWDMat2 : public MWDUniform {
public:
    MWDMat2(string nameinshader, string nameineditor, mat2 data)
        :MWDUniform(nameinshader, nameineditor) {
        m_data = data;
    }
public:
    virtual unsigned int GetUniformType() {
        return MAT2;
    }
    mat2 m_data;
};
class MWDMat3 : public MWDUniform {
public:
    MWDMat3(string nameinshader, string nameineditor, mat3 data)
        :MWDUniform(nameinshader, nameineditor) {
        m_data = data;
    }
public:
    virtual unsigned int GetUniformType() {
        return MAT3;
    }
    mat3 m_data;
};
class MWDMat4 : public MWDUniform {
public:
    MWDMat4(string nameinshader, string nameineditor, mat4 data)
        :MWDUniform(nameinshader, nameineditor) {
        m_data = data;
    }
public:
    virtual unsigned int GetUniformType() {
        return MAT4;
    }
    mat4 m_data;
};

