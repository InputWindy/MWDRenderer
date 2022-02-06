#pragma once
#include "headers.h"
#include "MWDMaterial.h"

//6个面都是空数据(texture_unit = 0)
struct EnvironmentMap {
public:
    int width, height, nrChannels;
    unsigned int ID;
public:
    EnvironmentMap(int wid = 600,int hei = 800,int channels=3) {
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
        for (unsigned int i = 0; i < 6; i++)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL
            );
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
    ~EnvironmentMap() {
        glDeleteTextures(1, &ID);
    }
    void Bind() {
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
    }
    void unBind() {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
    void operator=(const EnvironmentMap& texture) {
        width = texture.width;
        height = texture.height;
        nrChannels = texture.nrChannels;
        glDeleteTextures(1, &ID);
        ID = texture.ID;
    }
};

struct ShadowMap {
public:
    int width, height, nrChannels;
    unsigned int ID;
    unsigned int m_tex_unit;
    ShadowMap() {
        
    }
    ~ShadowMap() {

    }
    void Bind() {
        glActiveTexture(m_tex_unit);
        glBindTexture(GL_TEXTURE_2D, ID);
    }
    void unBind() {
        glActiveTexture(m_tex_unit);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};

//Pass维护的都是固定管线：光照贴图/阴影贴图/预计算
class MWDPass
{
public:
	enum
	{
		ShadowPass,
		LightPass,
		PreComputePass
	};
protected:
public:
	MWDPass() {
		
	}
	~MWDPass() {
	}
};

//阴影Pass需要维护一个光源
class MWDShadowPass :public MWDPass {
protected:
	
public:
	MWDShadowPass() {
		
	}
	~MWDShadowPass() {

	}
};

