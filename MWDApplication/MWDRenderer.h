#pragma once
#include "headers.h"
#include "MWDMesh.h"
#include "MWDRenderState.h"
#include "MWDCamera.h"
#include "MWDMaterial.h"
#include "MWDSkyBox.h"
#include "MWDPass.h"
static float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
};

struct FBO {
public:
	FBO() {
		glGenFramebuffers(1, &ID);
	}
	~FBO() {
		glDeleteFramebuffers(1, &ID);
	}
	unsigned int ID;
	void Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, ID);
	}
	void unBind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	//纹理维护颜色数据
	void AttachColorBufferi(GLuint texture,int i) {
		Bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, texture, 0);
		unBind();
	}
	//rbo维护深度模板数据
	void AttachRbo(GLuint rbo) {
		Bind();
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
		unBind();
	}
	bool CheckIsComplete() {
		Bind();
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
			unBind();
			return false;
		}
		else {
			unBind();
			return true;
		}
	}
};

struct ColorBuffer
{
public:
	ColorBuffer() {
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	~ColorBuffer() {
		glDeleteTextures(1, &ID);
	}
	unsigned int ID;
	void Bind() {
		glBindTexture(GL_TEXTURE_2D, ID);
	}
	void unBind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	//每帧设置宽高
	void SetWidthHeight(int wid,int hei) {
		glBindTexture(GL_TEXTURE_2D, ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wid, hei, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};

struct RBO
{
public:
	RBO() {
		glGenRenderbuffers(1, &ID);
	}
	~RBO() {
		glDeleteRenderbuffers(1, &ID);
	}
	unsigned int ID;
	void Bind() {
		glBindRenderbuffer(GL_RENDERBUFFER, ID);
	}
	void unBind() {
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
	//每帧设置宽高
	void SetWidthHeight(int wid, int hei) {
		glBindRenderbuffer(GL_RENDERBUFFER, ID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, wid, hei);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

};

struct ScreenMesh
{
public:
	ScreenMesh() {
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	~ScreenMesh() {
		glDeleteBuffers(1, &quadVBO);
		glDeleteVertexArrays(1, &quadVAO);
	}
	unsigned int quadVAO, quadVBO;
	void Bind() {
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	}
	void unBind() {
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
};

struct RenderContext {
public:
	RenderContext() {
		m_OffScreenRender = false;

		m_curCamera = NULL;
		m_curMesh = NULL;
		m_curMaterial = NULL;

		screen_width = 600;
		screen_height = 800;

		m_modelMatrix = mat4(1.0f);

		m_FBO.AttachColorBufferi(m_ColorBuffer.ID, 0);
		m_FBO.AttachRbo(m_ZSBuffer.ID);

		m_lightColor = vec3(1.0);

		m_environmentMap = NULL;
	}
	bool			m_OffScreenRender;		//是否开启离屏渲染

	MWDCamera*		m_curCamera;		//主相机
	MWDMesh*		m_curMesh;			//正在被渲染的Mesh
	MWDMaterial*	m_curMaterial;		//渲染Mesh所使用的材质

	unsigned int	screen_width;		//当前视口宽度
	unsigned int	screen_height;		//当前视口高度

	mat4			m_modelMatrix;      //Model矩阵

	FBO				m_FBO;				//离屏渲染帧缓冲（fbo）
	ColorBuffer		m_ColorBuffer;		//离屏渲染颜色缓冲（texture）
	RBO				m_ZSBuffer;			//离屏渲染的深度模板缓冲（rbo）
	ScreenMesh		m_screenMesh;		//屏幕矩形的Mesh

	MWDSkyBox*		m_curSkyBox;		//当前使用的天空盒（天空盒是渲染优先级最低的Mesh）

	vector<MWDPass*>	m_Pass;			//维护特殊渲染流程

	vec3				m_lightColor;		//点光源颜色

	vector<ShadowMap*>	m_shadowMap;		//每一个光源维护一个ShadowMap(可以同时传入多张ShadowMap)

	//当前Mesh所使用的环境贴图（由反射探针烘焙的）
	//环境贴图是一个Render()前的Pass，从Mesh位置处渲染CubeMap，
	//由Mesh对应的Material里的反射探针Pass维护这张贴图。
	//默认是天空盒对应的CubeMap
	EnvironmentMap*		m_environmentMap;	
};

//Renderer维护一个renderCtx,实现绘制一帧画面的所有方法
class MWDRenderer
{
	friend class RenderContex;
	friend class MWDMesh;
	friend class MWDMaterial;
protected:
	RenderContext ms_Ctx;
	static inline MWDRenderer* ms_Renderer = NULL;
	void PrintMat4(mat4& mat) {
		cout << endl;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				cout << mat[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
public:
	MWDRenderer() {

	}
	~MWDRenderer() {

	}

	//获取主渲染器
	static MWDRenderer* GetMainRenderer() {
		if (!ms_Renderer) {
			ms_Renderer = new MWDRenderer();
		}
		return ms_Renderer;
	}
	//遍历所有的Pass，每个Pass内执行一次完整的渲染。
	//除了主Pass需要填写实时Uniform，其他Pass都只需要填写内置Uniform
	static void UseMSAA() {
		glEnable(GL_MULTISAMPLE);
	}
	void DrawFrame() {
		AnalyzeRenderState();
		SetInnerUniform(ms_Ctx.m_curMaterial);
		DeliverMaterialUniform(ms_Ctx.m_curMaterial);
		Render();
	}
	void SetCamera(MWDCamera* camera) {
		assert(camera);
		ms_Ctx.m_curCamera = camera;
	}
	void SetScreenWidth(unsigned int _screen_width) {
		assert(_screen_width>0);
		ms_Ctx.screen_width = _screen_width;
	}
	void SetScreenHeight(unsigned int _screen_height) {
		assert(_screen_height>0);
		ms_Ctx.screen_height = _screen_height;
	}

	//设置天空盒的时候，也设置了所有Mesh默认的环境光贴图
	void SetSkyBox(MWDSkyBox* skyBox) {
		assert(skyBox);
		ms_Ctx.m_curSkyBox = skyBox;
	}
	//清空各种缓冲，渲染天空盒。烘焙光照贴图，光照探针，阴影贴图
	static void BeginRendering() {
		glClearColor(0.1, 0.1, 0.1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
		RenderContext* ms_Ctx = &GetMainRenderer()->ms_Ctx;
		if (ms_Ctx->m_OffScreenRender) {
			ms_Ctx->m_ZSBuffer.SetWidthHeight(ms_Ctx->screen_width, ms_Ctx->screen_height);
			ms_Ctx->m_ColorBuffer.SetWidthHeight(ms_Ctx->screen_width, ms_Ctx->screen_height);
			if (!ms_Ctx->m_FBO.CheckIsComplete()) {
				return;
			};
			ms_Ctx->m_FBO.Bind();
		}
		GetMainRenderer()->DrawSkyBox();
	}
	//执行屏幕后处理/延迟渲染
	static void EndRendering() {
		MWDShader* screenShader = MWDShader::GetScreenShader();
		RenderContext* ms_Ctx = &GetMainRenderer()->ms_Ctx;
		ms_Ctx->m_FBO.unBind();
		if (ms_Ctx->m_OffScreenRender) {
			//Pass2（绘制颜色缓冲）
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_STENCIL_TEST);
			glDisable(GL_BLEND);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |GL_STENCIL_BUFFER_BIT);

			screenShader->use();
			glActiveTexture(GL_TEXTURE0);
			ms_Ctx->m_ColorBuffer.Bind();
			screenShader->setInt("screenTexture", 0);

			ms_Ctx->m_screenMesh.Bind();
			glDrawArrays(GL_TRIANGLES, 0, 6);
			ms_Ctx->m_screenMesh.unBind();
		}
	}
private:
	//解析当前Mesh的渲染状态
	void AnalyzeRenderState() {
		#pragma region 填写渲染状态
		MWDRenderState render_state = ms_Ctx.m_curMaterial->m_renderState;
		ms_Ctx.m_OffScreenRender = render_state.m_OffScreenRender;
		if (render_state.m_bEnableDepth) {
			glEnable(GL_DEPTH_TEST);
			if (render_state.m_bDepthReadOnly) {
				glDepthMask(GL_FALSE);
			}
			glDepthFunc(render_state.m_CompareMethod);
		}
		if (render_state.m_bEnableBlend) {
			glEnable(GL_BLEND);
			glBlendEquation(render_state.m_uiBlendFuncMode);
			glBlendFunc(render_state.m_uiBlendSrc, render_state.m_uiBlendDest);
		}
		if (render_state.m_bEnableStencil) {
			glEnable(GL_STENCIL);
			if (render_state.m_bStencilReadOnly) {
				glStencilFunc(render_state.m_CompareMethod, render_state.m_uiStencilRef, 0x00);
			}
			else {
				glStencilFunc(render_state.m_CompareMethod, render_state.m_uiStencilRef, 0xFF);
			}

			glStencilOp(render_state.m_uiSFailOp, render_state.m_uiSPassZFailOp, render_state.m_uiPassOp);
		}
		if (render_state.m_bEnableCull) {
			glEnable(GL_CULL_FACE);
			glCullFace(render_state.m_uiCullMode);
			glFrontFace(render_state.m_uiFrontDefinition);
		}
		#pragma endregion
	}
	//传递Uniform变量
	void DeliverMaterialUniform(MWDMaterial* Material) {
		//↓不变的代码
		#pragma region 传递Uniform变量
		//填写Shader参数
		MWDMaterial* cur_Material = Material;
		MWDShader* cur_Shader = Material->m_shaderProgram;
		cur_Shader->use();
		for (vector<MWDUniform*>::iterator iter = cur_Material->m_uniform.begin(); iter < cur_Material->m_uniform.end(); ++iter) {
			MWDUniform* tmp = *iter;
			MWDUniform::UT type = (MWDUniform::UT)tmp->GetUniformType();
			switch (type)
			{
			case MWDUniform::INT:cur_Shader->setInt(tmp->nameInShader, ((MWDInt*)tmp)->m_data); break;
			case MWDUniform::FLOAT:cur_Shader->setFloat(tmp->nameInShader, ((MWDFloat*)tmp)->m_data); break;
			case MWDUniform::VEC2:cur_Shader->setVec2(tmp->nameInShader, ((MWDVec2*)tmp)->m_data); break;
			case MWDUniform::VEC3:cur_Shader->setVec3(tmp->nameInShader, ((MWDVec3*)tmp)->m_data); break;
			case MWDUniform::VEC4:cur_Shader->setVec4(tmp->nameInShader, ((MWDVec4*)tmp)->m_data); break;
			case MWDUniform::MAT2:cur_Shader->setMat2(tmp->nameInShader, ((MWDMat2*)tmp)->m_data); break;
			case MWDUniform::MAT3:cur_Shader->setMat3(tmp->nameInShader, ((MWDMat3*)tmp)->m_data); break;
			case MWDUniform::MAT4:cur_Shader->setMat4(tmp->nameInShader, ((MWDMat4*)tmp)->m_data); break;
			case MWDUniform::UT_MAX:
				break;
			default:
				break;
			}

		}
		for (vector<MWDTexture*>::iterator iter = cur_Material->m_sampler.begin(); iter < cur_Material->m_sampler.end(); ++iter) {
			MWDTexture* tmp = *iter;
			tmp->Bind();
			cur_Material->m_shaderProgram->setSampler(tmp->nameInEditor, tmp->m_tex_unit);
		}
		
		//填写内置纹理：环境贴图，shadowMap
		if (!ms_Ctx.m_environmentMap) {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, ms_Ctx.m_curSkyBox->ID);
			cur_Shader->setSampler("environment_map",0);
		}
		#pragma endregion
	}
	//所有Shader都可以传入同样的内置变量，传多了也没关系
	void SetInnerUniform(MWDMaterial* Material) {
		#pragma region 填写内置变量
		//shader内置变量传递（不需要editor内改变的uniform）
		//从相机读取vp矩阵。
		mat4 _projection = perspective(radians(ms_Ctx.m_curCamera->Zoom), (float)ms_Ctx.screen_width / ms_Ctx.screen_height, 0.1f, 100.0f);
		mat4 _view = ms_Ctx.m_curCamera->GetViewMatrix();
		Material->SetUniform<mat4, MWDMat4>(string("proj_matrix"), _projection);
		Material->SetUniform<mat4, MWDMat4>(string("view_matrix"), _view);
		Material->SetUniform<mat4, MWDMat4>(string("model_matrix"), ms_Ctx.m_modelMatrix);

		Material->SetUniform<vec3, MWDVec3>(string("lightPos"), vec3(-100,-100,-100));
		Material->SetUniform<vec3, MWDVec3>(string("lightColor"), ms_Ctx.m_lightColor);
		Material->SetUniform<vec3, MWDVec3>(string("viewPos"), ms_Ctx.m_curCamera->Position);
		#pragma endregion
	}
	//用当前的渲染状态和Shader来渲染当前的Mesh
	void Render() {
		glBindVertexArray(ms_Ctx.m_curMesh->VAO);
		glDrawElements(GL_TRIANGLES, ms_Ctx.m_curMesh->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	//渲染天空盒
	void DrawSkyBox() {
		if (!ms_Ctx.m_curSkyBox) {
			return;
		}
		else {
			glDepthMask(GL_FALSE);
			mat4 _projection = perspective(radians(ms_Ctx.m_curCamera->Zoom), (float)ms_Ctx.screen_width / ms_Ctx.screen_height, 0.1f, 100.0f);
			mat4 _view = glm::mat4(glm::mat3(ms_Ctx.m_curCamera->GetViewMatrix()));
			MWDShader* skyboxShader = ms_Ctx.m_curSkyBox->skyBoxShader;
			skyboxShader->use();
			glActiveTexture(GL_TEXTURE0);
			skyboxShader->setInt("skybox", 0);
			skyboxShader->setMat4("projection", _projection);
			skyboxShader->setMat4("view", _view);

			glBindVertexArray(GetMainRenderer()->ms_Ctx.m_curSkyBox->skyboxVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
		}
	}
};

