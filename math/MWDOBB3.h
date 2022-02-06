#pragma once
#include <headers.h>
namespace Engine {
	class  OBB3
	{
	public:
		//三个轴向
		vec3	m_A[3];	
		//三个轴向长度
		float	m_fA[3];	
		//中心
		vec3	m_Center;	
			
	public:
		OBB3() {
			
		};
		~OBB3() {

		};
		OBB3(const vec3 A[3], float fA[3], const vec3 Center) {
			Set(A, fA, Center);
		};
		OBB3(const vec3 A0, const vec3 A1, const vec3 A2,float fA0, float fA1, float fA2,const vec3 Center) {
			Set(A0, A1, A2, fA0, fA1, fA2, Center);
		};
		
		 void Set(const vec3 A[3], float fA[3], const vec3 Center) {
			m_A[0] = normalize(A[0]);
			m_A[1] = normalize(A[1]);
			m_A[2] = normalize(A[2]);
			m_fA[0] = abs(fA[0]);
			m_fA[1] = abs(fA[1]);
			m_fA[2] = abs(fA[2]);
			m_Center = Center;
		};
		 void Set(const vec3 A0, const vec3 A1, const vec3 A2,float fA0, float fA1, float fA2,const vec3 Center) {
			m_A[0] = normalize(A0);
			m_A[1] = normalize(A1);
			m_A[2] = normalize(A2);

			m_fA[0] = abs(fA0);
			m_fA[1] = abs(fA1);
			m_fA[2] = abs(fA2);
			m_Center = Center;
		};

		 vec3 GetCenter()const {
			return m_Center;
		};
		 
	};

}
