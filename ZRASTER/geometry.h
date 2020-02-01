#pragma once

#include <vector>
#include <cmath>

struct vec3d {
	float x, y, z;
};

struct triangle {
	vec3d p[3];
};

struct mesh {
	std::vector<triangle> tris;
};

struct mat4x4 {
	float m[4][4] = { 0 };
};


void multVec3Mat44(vec3d& i, vec3d& o, mat4x4& m);