#version 330 core
layout (location = 0) in vec3 aPos;

layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBiTangent;

layout (location = 4) in ivec4 aBoneIDs;
layout (location = 5) in vec4 aBoneWeights;

layout (location = 10) in vec2 TexCoords0;
	
out VS_OUT{
    vec3 vs_position;//世界空间坐标
    
    vec2 uv;         //uv
    
    mat3 TBN;        //TBN矩阵

    flat ivec4   BoneIDs;
    vec4    BoneWeights;
}vs_out;



//用于计算骨骼动画，修正世界空间坐标（实时传入）
uniform mat4 gWVP;

//顶点从模型空间转换到
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    
    vs_out.uv = TexCoords0;
    vs_out.BoneIDs = aBoneIDs;
    vs_out.BoneWeights = aBoneWeights;

    vec4 worldTangent = model * vec4(aTangent, 1.0);
    vec3 _worldTangent = vec3(worldTangent.xyz);
    vec4 worldBiTangent = model * vec4(aBiTangent, 1.0);
    vec3 _worldBiTangent = vec3(worldBiTangent.xyz);
    vec4 worldNormal = model * vec4(aNormal, 1.0);
    vec3 _worldNormal = vec3(worldNormal.xyz);
    
    vec4 worldPos = model * vec4(aPos, 1.0);
    vs_out.vs_position =vec3(worldPos.xyz); //世界空间坐标

    vs_out.TBN = mat3(_worldTangent,_worldBiTangent , _worldNormal);

    //加入骨骼动画后：gl_Position = projection * view * gWVP * model * vec4(aPos, 1.0);
    gl_Position = projection * view *model * vec4(aPos, 1.0);
}