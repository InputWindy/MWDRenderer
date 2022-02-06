#pragma once
#include "MWDLine3.h"
namespace math {
	class   Ray3 :public Line3
	{
	public:
		Ray3() {
			
		};
		Ray3( vec3 Orig,  vec3 Dir)	 {
			Line3::Set(Orig, Dir);
		};
		~Ray3() {

		};
	}; 
}