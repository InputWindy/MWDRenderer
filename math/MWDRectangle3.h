#pragma once
#include "MWDPlane3.h"
/********************************************************************************************


__________________
|        |        |
|    A[0]|        |
|		  |        |
|        |_A[1]___|
|                 |
|                 |
|_________________|


********************************************************************************************/
namespace math {
	class  Rectangle3 :public Plane3
	{
	public:
		//维护两个轴
		vec3	m_A[2];
		//维护两个轴向的长度
		float	m_fA[2];
		//维护矩形中心
		vec3	m_Center;
	public:
		Rectangle3() {
			m_Center = m_A[0] = m_A[1]=  vec3(0.0f);
			m_fA[0] = m_fA[1] = 0.0f;
		};
		~Rectangle3() {

		};
		Rectangle3(const vec3 Center, const vec3 A0, const vec3 A1, float fA0, float fA1) {
			Set(Center, A0, A1, fA0, fA1);
		};
		Rectangle3(const vec3 Center, const vec3 A[2], const float fA[2]) {
			Set(Center, A, fA);
		};
		void Set(const vec3 Center, const vec3 A0, const vec3 A1, float fA0, float fA1) {
			m_Center = Center;
			m_A[0] = normalize(A0);
			m_A[1] = normalize(A1);
			m_fA[0] = abs(fA0);
			m_fA[1] = abs(fA1);
			Plane3::Set(cross(A0, A1), Center);
		};
		void Set(const vec3 Center, const vec3 A[2], const float fA[2]) {
			Set(Center, A[0], A[1], fA[0], fA[1]);
		};
	};

}