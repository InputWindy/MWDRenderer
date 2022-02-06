#pragma once
#include "MWDAABB3.h"
#include "MWDCylinder3.h"
#include "MWDLine3.h"
#include "MWDOBB3.h"
#include "MWDPlane3.h"
#include "MWDRay3.h"
#include "MWDRectangle3.h"
#include "MWDSegment3.h"
#include "MWDSphere3.h"
#include "MWDTriangle3.h"
#define GetVec3Length(_vec3) \
sqrt(_vec3.x* _vec3.x+ _vec3.y * _vec3.y+ _vec3.z * _vec3.z)

#define GetVec3SqrLen(_vec3) \
_vec3.x* _vec3.x+ _vec3.y * _vec3.y+ _vec3.z * _vec3.z