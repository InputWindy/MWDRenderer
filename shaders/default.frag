#version 330 core
//常量
const float PI = 3.1415926535;
const int MAX_POINT_LIGHT_NUM = 4;
const int MAX_SPOT_LIGHT_NUM = 4;
const int MAX_AREA_LIGHT_NUM = 4;

out vec4 FragColor;

struct DirectionalLight{
    vec3 Color;
    vec3 Direction;
};

struct PointLight{
    vec3 Color;
    vec3 Position;
    float CutOff;
};

struct SpotLight{
    vec3 Color;
    vec3 Position;
    vec3 Direction;
    float Angle;
    float CutOff;
};

struct AreaLight{
    vec3 Color;
    vec3 Position;
    vec3 Normal;
    float width;
    float height;
    float CutOff;
};

struct Camera{
    vec3 Position;
};

in VS_OUT{
    vec3 vs_position;//世界空间坐标
    
    vec2 uv;         //uv
    
    mat3 TBN;        //TBN矩阵
}fs_in;

//暂时使用的内置变量，之后需要替换成光源和相机
uniform vec3 lightPos;
uniform vec3 lightColor;

//内置变量（最多一个太阳，最多4个灯泡，最多4个手电筒，最多4个发光平面,一台摄像机）
uniform DirectionalLight sunLight;
uniform int PointLightNum;//灯光数量
uniform PointLight pointLights[MAX_POINT_LIGHT_NUM];
uniform int SpotLightNum;//手电筒数量
uniform SpotLight spotLights[MAX_SPOT_LIGHT_NUM];
uniform int AreaLightNum;//手电筒数量
uniform AreaLight AreaLights[MAX_AREA_LIGHT_NUM];
uniform Camera camera;

uniform samplerCube environment_map;//环境光照（由反射探针或天空盒提供）

//手动传入的参数（PBR纹理）
uniform sampler2D diffuse_map;
uniform sampler2D normal_map;
uniform sampler2D metallic_map;
uniform sampler2D ao_map;
uniform sampler2D roughness_map;

//H:半角向量。roughness：粗糙度。N:顶点法线——NDF方程
float D_GGX_TR(vec3 N, vec3 H, float roughness)
{
    float a2     = roughness*roughness;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom    = a2;
    float denom  = (NdotH2 * (a2 - 1.0) + 1.0);
    denom        = PI * denom * denom;
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float k)
{
    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return nom / denom;
}

//几何函数G：计算微表面模型自遮挡比率   k是粗糙度的重映射   N：法线   V:视线  L:光线
float GeometrySmith(vec3 N, vec3 V, vec3 L, float k)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, k);
    float ggx2 = GeometrySchlickGGX(NdotL, k);

    return ggx1 * ggx2;
}

//菲涅尔项
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

//计算光源强度衰减（平方反比）
vec3 CalRadiance(vec3 light_color,float _distance)
{
    float attenuation = 1.0 / (_distance * _distance);
    return light_color*attenuation;
}

//以下计算全部在世界空间下
void main()
{   
    //法线
    vec3 normal = texture(normal_map, fs_in.uv).rgb;
    normal = normalize(normal * 2.0 - 1.0);   
    normal = normalize(fs_in.TBN * normal);
    //环境光强度
    vec3 irradiance = texture(environment_map,normal).rgb;
    //反照率(sRGB)
    vec3  albedo =pow(texture(diffuse_map, fs_in.uv).rgb,vec3(2.2));
    //金属度（线性空间）
    float metallic = texture(metallic_map, fs_in.uv).r;
    //粗糙度（线性空间）
    float roughness = texture(roughness_map, fs_in.uv).r;
    //ao（线性空间）
    float ao = texture(ao_map, fs_in.uv).r;
    // 入射光方向（指向光源）
    vec3 lightDir = normalize(lightPos - fs_in.vs_position);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);//阴影bias（暂时无用）
    //视线方向
    vec3 viewDir = normalize(camera.Position - fs_in.vs_position);
    //反射方向
    vec3 reflectDir = reflect(-lightDir, normal);
    //半角向量
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    //====================以下进行pbr光照计算===============//
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    vec3 Lo = vec3(0.0);

    vec3 F  = fresnelSchlick(max(dot(halfwayDir, viewDir), 0.0), F0);
    float NDF = D_GGX_TR(normal, halfwayDir, roughness);       
    float G   = GeometrySmith(normal, viewDir, lightDir, roughness);  
    vec3  nominator    = NDF * G * F;
    float denominator  = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.001; 
    vec3  specular     = nominator / denominator; 

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic; 

    float NdotL = max(dot(normal, lightDir), 0.0);  

    vec3 fr = ( kD*albedo/PI + kS*specular);
    //Lo只是所有光源给到的直接光照。
    //间接光照要由irradiance求蒙特卡洛积分
    //
    Lo += fr * lightColor * NdotL;
    
    FragColor   = vec4( Lo*5 ,1.0);
    //====================以下进行gamma校正===============//
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/2.2));//颜色修改为sRGB空间
}