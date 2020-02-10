#pragma once

#include <vector>
#include <cmath>

#include <fstream>
#include <strstream>

struct vec3d {
	float x;
	float y;
	float z;
	float w;
public:
	vec3d(float a, float b, float c) : x(a), y(b), z(c), w(1) { }
	vec3d() : vec3d(0.0f, 0.0f, 0.0f) {}
};

struct triangle {
	vec3d p[3];

	uint32_t color;
};

struct mesh {
	std::vector<triangle> tris;

	bool loadfromobj(std::string file);
};

struct mat4x4 {
	float m[4][4] = { 0 };
};


void multVec3Mat44(vec3d& i, vec3d& o, mat4x4& m);

vec3d vaddvector(vec3d& v1, vec3d& v2);
vec3d vsubvector(vec3d& v1, vec3d& v2);
vec3d smulvector(vec3d& v1, float k);
vec3d sdivvector(vec3d& v1, float k);
float vdotproduct(vec3d& v1, vec3d& v2);
float vlenght(vec3d& v);
vec3d vnormalise(vec3d& v);
vec3d vcrossproduct(vec3d& v1, vec3d& v2);
vec3d mmulvec3mat44(mat4x4& m, vec3d& i);
mat4x4 mnewrotationmatrix_x(float fTheta);
mat4x4 mnewrotationmatrix_y(float fTheta);
mat4x4 mnewrotationmatrix_z(float fTheta);
mat4x4 mnewtranslationmatrix(float x, float y, float z);
mat4x4 mnewprojectionmatrix(float fFovDegrees, float fAspectRatio, float fNear, float fFar);
mat4x4 mnewidentitymatrix();
mat4x4 mmulmat44mat44(mat4x4& m1, mat4x4& m2);
