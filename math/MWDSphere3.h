#pragma once
#include "MWDAABB3.h"
namespace math {
	class  Sphere3
	{
	public:
		vec3 m_Center;		
		float m_fRadius;		
		//默认生成单位球
		Sphere3() {
			m_Center = vec3(0.0f);
			m_fRadius = 1.0f;
		};
		~Sphere3() {

		};
		Sphere3(const vec3 Center, float fRadius) {
			m_fRadius = fRadius;
			m_Center = Center;
		};

		//用点集生成球包围盒
		static Sphere3 CreateSphere(const vec3* pPointArray, unsigned int uiPointNum) {
			if (!pPointArray || !uiPointNum)
				return Sphere3();
			vec3 Sum,m_Center;
			for (unsigned int i = 0; i < uiPointNum; i++)
				Sum += pPointArray[i];
			m_Center = Sum / (uiPointNum * 1.0f);
			float radiusSqr = 0.0f;
			for (unsigned int i = 0; i < uiPointNum; i++)
			{
				vec3 diff = pPointArray[i] - m_Center;
				float Temp = diff.x*diff.x+ diff.y * diff.y + diff.z * diff.z ;
				if (Temp > radiusSqr)
					radiusSqr = Temp;

			}
			float m_fRadius = sqrt(radiusSqr);
		};
		
		//合并两个球形包围盒
		static Sphere3 MergSpheres(const Sphere3 Sphere1, const Sphere3 Sphere2 ) {
			Sphere3 Temp;
			vec3 CenterDiff = Sphere1.m_Center - Sphere2.m_Center;
			float fRadiusDiff = Sphere1.m_fRadius - Sphere2.m_fRadius;
			float fRadiusDiffSqr = fRadiusDiff * fRadiusDiff;
			float Lsqr = CenterDiff.x* CenterDiff.x+ CenterDiff.y * CenterDiff.y + CenterDiff.z * CenterDiff.z ;
			if (fRadiusDiffSqr >= Lsqr)
			{
				if (fRadiusDiff >= 0.0f)
				{
					Temp.m_Center = Sphere1.m_Center;
					Temp.m_fRadius = Sphere1.m_fRadius;
				}
				else
				{
					Temp.m_Center = Sphere2.m_Center;
					Temp.m_fRadius = Sphere2.m_fRadius;
				}

			}
			else
			{
				float L = sqrt(Lsqr);
				float t = (L + Sphere1.m_fRadius - Sphere2.m_fRadius) / (2.0f * L);
				Temp.m_Center = Sphere2.m_Center + CenterDiff * t;
				Temp.m_fRadius = (L + Sphere1.m_fRadius + Sphere2.m_fRadius) / 2.0f;
			}

			return Temp;
		};
		
		//获取球体的AABB3
		AABB3 GetAABB() {
			vec3 Max = m_Center + vec3(m_fRadius, m_fRadius, m_fRadius);
			vec3 Min = m_Center + vec3(-m_fRadius, -m_fRadius, -m_fRadius);
			return AABB3(Max, Min);
		};
	};

}
