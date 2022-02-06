#pragma once
#include "MWDRay3.h"
namespace math {
	class  Segment3 :public Ray3
	{
	public:
		//Ä©Î²
		vec3		m_End;
		//³¤¶È
		float		m_fLen;
	public:
		Segment3() {
			m_End = vec3(0.0f);
			m_fLen = 0.0f;
		};
		~Segment3() {

		};
		Segment3( vec3 Orig,  vec3 End) {
			Set(Orig, End);
		};
		Segment3( vec3 Orig,  vec3 Dir, float fLen) {
			Set(Orig, Dir, fLen);
		};
		void Set( vec3 Orig,  vec3 End) {
			vec3 Dir = End - Orig;
			m_fLen =sqrt(Dir.x*Dir.x+ Dir.y * Dir.y + Dir.z * Dir.z );
			Line3::Set(Orig, Dir);
		};
		void SetEnd(  vec3 End) {
			vec3 Dir = End -m_Orig, Orig = m_Orig;
			m_fLen = sqrt(Dir.x * Dir.x + Dir.y * Dir.y + Dir.z * Dir.z);
			Line3::Set(Orig, Dir);
		};
		void SetLen(float fLen) {
			vec3 Dir = m_End -m_Orig, Orig = m_Orig;
			m_fLen = sqrt(Dir.x * Dir.x + Dir.y * Dir.y + Dir.z * Dir.z);
			Line3::Set(Orig, Dir);
		};
		void Set( vec3 Orig,  vec3 Dir, float fLen) {
			Line3::Set(Orig, Dir);
			m_fLen = fLen;
			m_End = Orig + (Ray3::m_Dir * m_fLen);
		};

	};

}
