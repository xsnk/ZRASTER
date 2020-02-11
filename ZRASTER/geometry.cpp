#include "geometry.h"


vec3d vaddvector(vec3d& v1, vec3d& v2)
{
	return vec3d(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

vec3d vsubvector(vec3d& v1, vec3d& v2)
{
	return vec3d(float(static_cast<float>(v1.x) - static_cast<float>(v2.x)), float(v1.y - v2.y), float(v1.z - v2.z));
}

vec3d smulvector(vec3d& v1, float k)
{
	return vec3d(v1.x * k, v1.y * k, v1.z * k);
}

vec3d sdivvector(vec3d& v1, float k)
{
	return vec3d(v1.x / k, v1.y / k, v1.z / k);
}

float vdotproduct(vec3d& v1, vec3d& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float vlenght(vec3d& v)
{
	return sqrtf(vdotproduct(v, v));
}

vec3d vnormalise(vec3d& v)
{
	float l = vlenght(v);
	return vec3d(v.x / l, v.y / l, v.z / l);
}

vec3d vcrossproduct(vec3d& v1, vec3d& v2)
{
	vec3d v;
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return v;
}

vec3d vmulvec3mat44(mat4x4& m, vec3d& i)
{
	vec3d v;
	v.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + i.w * m.m[3][0];
	v.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + i.w * m.m[3][1];
	v.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + i.w * m.m[3][2];
	v.w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + i.w * m.m[3][3];
	return v;
}

mat4x4 mnewidentitymatrix()
{
	mat4x4 mat;
	mat.m[0][0] = 1.0f;
	mat.m[1][1] = 1.0f;
	mat.m[2][2] = 1.0f;
	mat.m[3][3] = 1.0f;
	return mat;
}

mat4x4 mnewrotationmatrix_x(float fTheta)
{
	mat4x4 mat;
	mat.m[0][0] = 1.0f;
	mat.m[1][1] = cosf(fTheta);
	mat.m[1][2] = -sinf(fTheta);
	mat.m[2][1] = sinf(fTheta);
	mat.m[2][2] = cosf(fTheta);
	mat.m[3][3] = 1.0f;
	return mat;
}

mat4x4 mnewrotationmatrix_y(float fTheta)
{
	mat4x4 mat;
	mat.m[0][0] = cosf(fTheta);
	mat.m[0][2] = sinf(fTheta);
	mat.m[2][0] = -sinf(fTheta);
	mat.m[2][2] = cosf(fTheta);
	mat.m[1][1] = 1.0f;
	mat.m[3][3] = 1.0f;
	return mat;
}

mat4x4 mnewrotationmatrix_z(float fTheta)
{
	mat4x4 mat;
	mat.m[0][0] = cosf(fTheta);
	mat.m[0][1] = sinf(fTheta);
	mat.m[1][0] = -sinf(fTheta);
	mat.m[1][1] = cosf(fTheta);
	mat.m[2][2] = 1.0f;
	mat.m[3][3] = 1.0f;
	return mat;
}

mat4x4 mnewtranslationmatrix(float x, float y, float z)
{
	mat4x4 mat;
	mat.m[0][0] = 1.0f;
	mat.m[1][1] = 1.0f;
	mat.m[2][2] = 1.0f;
	mat.m[3][3] = 1.0f;
	mat.m[3][0] = x;
	mat.m[3][1] = y;
	mat.m[3][2] = z;
	return mat;
}

mat4x4 mnewprojectionmatrix(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
{
	mat4x4 mat;
	float fFovRad = 1.0 / tanf(fFovDegrees * 0.5f / 180.0f * 3.14159f);
	mat.m[0][0] = fAspectRatio * fFovRad;
	mat.m[1][1] = fFovRad;
	mat.m[2][2] = fFar / (fFar - fNear);
	mat.m[3][2] = (-fFar * fNear) / (fFar - fNear);
	mat.m[2][3] = 1.0f;
	mat.m[3][3] = 1.0f;
	return mat;
}

mat4x4 mmulmat44mat44(mat4x4& m1, mat4x4& m2)
{
	mat4x4 mat;
	for (int c = 0; c < 4; c++) {
		for (int r = 0; r < 4; r++) {
			mat.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
		}
	}
	return mat;
}

mat4x4 mpointat(vec3d& pos, vec3d& target, vec3d& up)
{
	// Calculate new forward direction
	vec3d newForward = vsubvector(target, pos);
	newForward = vnormalise(newForward);

	// Calculate new Up direction
	vec3d a = smulvector(newForward, vdotproduct(up, newForward));
	vec3d newUp = vsubvector(up, a);
	newUp = vnormalise(newUp);

	// New Right direction is easy, its just cross product
	vec3d newRight = vcrossproduct(newUp, newForward);

	// Construct Dimensioning and Translation Matrix	
	mat4x4 matrix;
	matrix.m[0][0] = newRight.x;	matrix.m[0][1] = newRight.y;	matrix.m[0][2] = newRight.z;	matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = newUp.x;		matrix.m[1][1] = newUp.y;		matrix.m[1][2] = newUp.z;		matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = newForward.x;	matrix.m[2][1] = newForward.y;	matrix.m[2][2] = newForward.z;	matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = pos.x;			matrix.m[3][1] = pos.y;			matrix.m[3][2] = pos.z;			matrix.m[3][3] = 1.0f;
	return matrix;
}

mat4x4 mquickinverserotmat(mat4x4& m) // Only for Rotation/Translation Matrices
{
	mat4x4 matrix;
	matrix.m[0][0] = m.m[0][0]; matrix.m[0][1] = m.m[1][0]; matrix.m[0][2] = m.m[2][0]; matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = m.m[0][1]; matrix.m[1][1] = m.m[1][1]; matrix.m[1][2] = m.m[2][1]; matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = m.m[0][2]; matrix.m[2][1] = m.m[1][2]; matrix.m[2][2] = m.m[2][2]; matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = -(m.m[3][0] * matrix.m[0][0] + m.m[3][1] * matrix.m[1][0] + m.m[3][2] * matrix.m[2][0]);
	matrix.m[3][1] = -(m.m[3][0] * matrix.m[0][1] + m.m[3][1] * matrix.m[1][1] + m.m[3][2] * matrix.m[2][1]);
	matrix.m[3][2] = -(m.m[3][0] * matrix.m[0][2] + m.m[3][1] * matrix.m[1][2] + m.m[3][2] * matrix.m[2][2]);
	matrix.m[3][3] = 1.0f;
	return matrix;
}

bool mesh::loadfromobj(std::string file)
{
	std::ifstream f(file);
	if (!f.is_open()) return false;

	// temprory pool of vertices
	std::vector<vec3d> verts;

	while (!f.eof()) {
		char line[128];
		f.getline(line, 128);

		std::strstream s;
		s << line;

		char junk;

		if (line[0] == 'v') {
			vec3d v;
			s >> junk >> v.x >> v.y >> v.z;
			verts.push_back(v);
		}

		if (line[0] == 'f') {
			int f[3];
			s >> junk >> f[0] >> f[1] >> f[2];
			tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
		}

	}

	return true;
}
