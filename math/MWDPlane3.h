#pragma once
#include <headers.h>
namespace math {
	class   Plane3
	{
	public: 
		vec3	m_N;        // 法线
		float	m_fD;       // (ax+by+cz+d=0)
	public:
		Plane3() {
			m_fD = 0.0f;
			m_N = vec3(0.0f);
		};
		//一点，一法线
		Plane3(const vec3 N, const vec3 P) {
			Set(N, P);
		};
		//三点一面
		Plane3(const vec3 P0, const vec3 P1, const vec3 P2) {
			Set(P0, P1, P2);
		};
		//三点一面
		Plane3(const vec3 Point[3]) {
			Set(Point[0], Point[1], Point[2]);
		};
		//法线和D
		Plane3(const vec3 N, float fD) {
			Set(N, fD);
		};
		void  Set(const vec3 N, const vec3 P) {
			m_N = normalize(N);
			m_fD = dot(m_N, P);
		};
		void  Set(const vec3 N, float fD) {
			float Len =sqrt( N.x*N.x+ N.y * N.y+ N.z * N.z);
			m_N = N / Len;
			m_fD = fD / Len;
		};
		void  Set(const vec3 P0, const vec3 P1, const vec3 P2) {
			vec3 vcEdge1 = P1 - P0;
			vec3 vcEdge2 = P2 - P0;
			cross(vcEdge1, vcEdge2);
			m_N = normalize(m_N);
			m_fD = -dot(m_N,P0);
		};
		void  Set(const vec3 Point[3]) {
			Set(Point[0], Point[1], Point[2]);
		};
		vec3 GetReflectDir(const vec3 Dir)const {
			vec3 TempN = m_N * (-1.0f),dir = Dir;
			return Dir - m_N * dot(dir,m_N) * 2.0f;
		};
		
	}; 

}
