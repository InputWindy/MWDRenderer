#pragma once
#include <headers.h>
namespace math {
	class  AABB3
	{
	public:
		//三个轴向(x,y,z)
		static inline const vec3 m_A[3] = {vec3(1.0,0.0,0.0),vec3(0.0,1.0,0.0) ,vec3(0.0,0.0,1.0) } ;
			
		vec3	m_Center;					
		vec3	m_Max;
		vec3	m_Min;
	public:

		AABB3() {
			m_Center = m_Max = m_Min = vec3(0.0f);
		};
		~AABB3() {

		};

		AABB3(const vec3 Max, const vec3 Min) {
			Set(Max, Min);
		};
		
		//给一个点集，计算AABB3
		static AABB3 CreateAABB(const vec3* pPointArray, unsigned int uiPointNum) {
			vec3 m_Min, m_Max;
			if (!pPointArray || !uiPointNum)
				return AABB3();
			m_Min = pPointArray[0];
			m_Max = pPointArray[0];
			for (unsigned int i = 1; i < uiPointNum; i++)
			{
				if (pPointArray[i].x < m_Min.x)
				{
					m_Min.x = pPointArray[i].x;
				}
				else if (pPointArray[i].x > m_Max.x)
				{
					m_Max.x = pPointArray[i].x;
				}

				if (pPointArray[i].y < m_Min.y)
				{
					m_Min.y = pPointArray[i].y;
				}
				else if (pPointArray[i].y > m_Max.y)
				{
					m_Max.y = pPointArray[i].y;
				}

				if (pPointArray[i].z < m_Min.z)
				{
					m_Min.z = pPointArray[i].z;
				}
				else if (pPointArray[i].z > m_Max.z)
				{
					m_Max.z = pPointArray[i].z;
				}
			}
			return AABB3(m_Max, m_Min);
		};
		
		void Set(const vec3 Max, const vec3 Min) {
			m_Max = Max;
			m_Min = Min;
			m_Center = (Max + Min) / 2.0f;
		};
		
		const vec3 GetCenter()const {
			return m_Center;
		};
		const vec3 GetMaxPoint()const {
			return m_Max;
		};
		const vec3 GetMinPoint()const {
			return m_Min;
		};
	};

}