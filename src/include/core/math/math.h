#pragma once

//Note : Custom math library currently not in use. Used as reference alone
//Note : Projection calculation broken

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"

#include "matrix3x3.h"
#include "matrix4x4.h"

#include "matrix_transform.h"

#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using mat4 = glm::mat4;
using mat3 = glm::mat3;
using vec3 = glm::vec3;
using vec4 = glm::vec4;

#define PI 3.141592f

//Utility functions
namespace halogen::math
{
	inline float radians_to_degrees(float radians)
	{
		float result = radians * 180.0f / PI;
		return result;
	}

	inline float degrees_to_radians(float degrees)
	{
		float result = degrees * PI / 180.0f;
		return result;
	}
}