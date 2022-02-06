#pragma once
#include "headers.h"
#include "MWDShader.h"
const float skyboxVertices[] = {
    // 前面    
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    //背面
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    //右面
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     //左面
    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,
    //上面
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
    //下面
    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};
class MWDSkyBox
{
    friend class MWDRenderer;
protected:
	vector<string> m_CubeFaces;
	int width, height, nrChannels;
	unsigned int ID,skyboxVAO, skyboxVBO;
    MWDShader* skyBoxShader;
public:
	MWDSkyBox(string px,string nx,string py,string ny,string pz,string nz) {
		m_CubeFaces.push_back(px);
		m_CubeFaces.push_back(nx);
		m_CubeFaces.push_back(py);
		m_CubeFaces.push_back(ny);
		m_CubeFaces.push_back(pz);
		m_CubeFaces.push_back(nz);
        //stbi_set_flip_vertically_on_load(true);
        ID = load();
        
        glGenVertexArrays(1, &skyboxVAO);
        glGenBuffers(1, &skyboxVBO);
        glBindVertexArray(skyboxVAO);
        glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        skyBoxShader = MWDShader::GetSkyBoxShader();
	}
	~MWDSkyBox() {
        glDeleteTextures(1, &ID);
        glDeleteVertexArrays(1, &skyboxVAO);
        glDeleteBuffers(1, &skyboxVBO);
        delete skyBoxShader;
	}
    //获取一个天空盒实例
    static MWDSkyBox* GetDefaultSkyBox() {
        MWDSkyBox* skybox = NULL;
        skybox = new MWDSkyBox(
            "C:/Users/InputWindy/Desktop/MWDRenderer/img/skybox/right.jpg",
            "C:/Users/InputWindy/Desktop/MWDRenderer/img/skybox/left.jpg", 
            "C:/Users/InputWindy/Desktop/MWDRenderer/img/skybox/bottom.jpg", 
            "C:/Users/InputWindy/Desktop/MWDRenderer/img/skybox/top.jpg", 
            "C:/Users/InputWindy/Desktop/MWDRenderer/img/skybox/front.jpg", 
            "C:/Users/InputWindy/Desktop/MWDRenderer/img/skybox/back.jpg");
        return skybox;
    }
private:
	unsigned int load() {
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

        for (unsigned int i = 0; i < m_CubeFaces.size(); i++)
        {
            unsigned char* data = stbi_load(m_CubeFaces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap texture failed to load at path: " << m_CubeFaces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return ID;
	}
};

