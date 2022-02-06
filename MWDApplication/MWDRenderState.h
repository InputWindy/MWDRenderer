#pragma once
#include "headers.h"
class MWDRenderState
{
public:
	enum {
		CM_ALWAYS = GL_ALWAYS,
		CM_NEVER = GL_NEVER,
		CM_LESS = GL_LESS,
		CM_EQUAL = GL_EQUAL,
		CM_LEQUAL = GL_LEQUAL,
		CM_GREATER = GL_GREATER,
		CM_NOTEQUAL = GL_NOTEQUAL,
		CM_GEQUAL = GL_GEQUAL
	};
	enum {
		OP_KEEP = GL_KEEP,
		OP_ZERO = GL_ZERO,
		OP_REPLACE = GL_REPLACE,
		OP_INCR = GL_INCR,
		OP_INCR_WRAP = GL_INCR_WRAP,
		OP_DECR = GL_DECR,
		OP_DECR_WRAP = GL_DECR_WRAP,
		OP_INVERT = GL_INVERT
	};
	enum {
		BF_ZERO = GL_ZERO,
		BF_ONE = GL_ONE,
		BF__SRC_COLOR = GL_SRC_COLOR,
		BF_ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
		BF_DST_COLOR = GL_DST_COLOR,
		BF_ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
		BF_SRC_ALPHA = GL_SRC_ALPHA,
		BF_ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
		BF_DST_ALPHA = GL_DST_ALPHA,
		BF_ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
		BF_CONSTANT_COLOR = GL_CONSTANT_COLOR,
		BF_ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR,
		BF_CONSTANT_ALPHA = GL_CONSTANT_ALPHA,
		BF_ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA
	};
	enum {
		FM_ADD = GL_FUNC_ADD,
		FM_SUB = GL_FUNC_SUBTRACT,
		FM_R_SUB = GL_FUNC_REVERSE_SUBTRACT
	};
	enum {
		FD_CCW = GL_CCW,
		FD_CW = GL_CW
	};
	enum {
		CM_FRONT = GL_FRONT,
		CM_BACK = GL_BACK,
		CM_FRONT_AND_BACK = GL_FRONT_AND_BACK
	};
public:

	//获取不透明，不描边物体的渲染状态
	static MWDRenderState GetDefault() {
		return MWDRenderState();
	}
	//获取不透明，不描边物体的渲染状态
	static MWDRenderState GetTransparentRenderState() {
		MWDRenderState defaultRenderState = MWDRenderState();
		defaultRenderState.m_bEnableDepth = false;
		defaultRenderState.m_bEnableBlend = true;
		defaultRenderState.m_bEnableCull = false;
		return defaultRenderState;
	}
public:
	MWDRenderState() {
		m_OffScreenRender = false;
		m_CompareMethod = CM_LESS;

		m_bEnableDepth = true;
		m_bDepthReadOnly = false;

		m_bEnableStencil = false;
		m_bStencilReadOnly = true;
		m_uiSFailOp = OP_KEEP;
		m_uiSPassZFailOp = OP_KEEP;
		m_uiPassOp = OP_KEEP;
		m_uiStencilRef = 1;

		m_bEnableBlend = false;
		m_uiBlendFuncMode = FM_ADD;
		m_uiBlendSrc = BF_SRC_ALPHA;
		m_uiBlendDest = BF_DST_ALPHA;

		m_bEnableCull = true;
		m_uiFrontDefinition = FD_CCW;
		m_uiCullMode = CM_BACK;
	}
	void operator=(const MWDRenderState& rs) {
		m_OffScreenRender = rs.m_OffScreenRender;
		m_CompareMethod = rs.m_CompareMethod;

		m_bEnableDepth = rs.m_bEnableDepth;
		m_bDepthReadOnly = rs.m_bDepthReadOnly;

		m_bEnableStencil = rs.m_bEnableStencil;
		m_bStencilReadOnly = rs.m_bStencilReadOnly;
		m_uiSFailOp = rs.m_uiSFailOp;
		m_uiSPassZFailOp = rs.m_uiSPassZFailOp;
		m_uiPassOp = rs.m_uiPassOp;
		m_uiStencilRef = rs.m_uiStencilRef;

		m_bEnableBlend = rs.m_bEnableBlend;
		m_uiBlendFuncMode = rs.m_uiBlendFuncMode;
		m_uiBlendSrc = rs.m_uiBlendSrc;
		m_uiBlendDest = rs.m_uiBlendDest;

		m_bEnableCull = rs.m_bEnableCull;
		m_uiFrontDefinition = rs.m_uiFrontDefinition;
		m_uiCullMode = rs.m_uiCullMode;
	}

	bool		 m_OffScreenRender;
	unsigned int m_CompareMethod;

	//depth
	bool m_bEnableDepth;
	bool m_bDepthReadOnly;


	//stencil
	bool m_bEnableStencil;
	bool m_bStencilReadOnly;
	unsigned int m_uiSFailOp;
	unsigned int m_uiSPassZFailOp;
	unsigned int m_uiPassOp;
	unsigned int m_uiStencilRef;

	//blend
	bool m_bEnableBlend;
	unsigned int m_uiBlendFuncMode;
	unsigned int m_uiBlendSrc;
	unsigned int m_uiBlendDest;

	//Cull
	bool m_bEnableCull;
	unsigned int m_uiFrontDefinition;
	unsigned int m_uiCullMode;
};

