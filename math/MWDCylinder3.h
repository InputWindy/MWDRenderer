#pragma once
#include <headers.h>
namespace math {
	class  Cylinder3
	{
	public:
		vec3		m_Center;
		vec3		m_Dir;
		float		m_fRadius;
		float		m_fHalfHeight;
	public:
		Cylinder3() {
			m_Center = vec3(0.0f);
			m_Dir = vec3(0.0f, 1.0f, 0.0f);
			m_fRadius = 1.0f;
			m_fHalfHeight = 2.0f;
		};
		Cylinder3(vec3 Center, vec3 Dir, float fRadius, float fHalfHeight) {
			m_Center = Center;
			m_Dir = Dir;
			m_fRadius = fRadius;
			m_fHalfHeight = fHalfHeight;
		};
		~Cylinder3() {
			
		};
	};
}
