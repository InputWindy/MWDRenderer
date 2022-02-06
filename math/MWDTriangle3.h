#pragma once
#include "MWDPlane3.h"
/***************************************************************************************

					fTriangleParameter[1]
		   m_V[0]	_________________________m_V[1]
				   \                       /
					\                     /
					 \                   /
					  \                 /
					   \               /
						\             /
fTriangleParameter[2]  \           /  fTriangleParameter[0]
						  \         /
						   \       /
							 \    /
							  \  /
							   \/m_V[2]
***************************************************************************************/
namespace math
{
	class Triangle3 :public Plane3
	{
		friend class MWD;
	public:
		//维护三角形的三个顶点
		vec3 m_V[3];
	public:
		Triangle3() {
			m_V[0] = m_V[1] = m_V[2] = vec3(0.0f);
		};
		~Triangle3() {

		};
		Triangle3(const vec3 P0, const vec3 P1, const vec3 P2) {
			Set(P0, P1, P2);
		};
		Triangle3(const vec3 Point[3]) {
			Set(Point);
		};
		void Set(const vec3 P0, const vec3 P1, const vec3 P2) {
			m_V[0] = P0;
			m_V[2] = P2;
			m_V[1] = P1;
			Plane3::Set(m_V);
		};
		void Set(const vec3 Point[3]) {
			m_V[0] = Point[0];
			m_V[2] = Point[2];
			m_V[1] = Point[1];
			Plane3::Set(m_V);
		};
		
	};

};