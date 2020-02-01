#include "zraster.h"
#include "geometry.h"


int main()
{
	ZRASTER app;
	if (!app.init("ZRASTER", WIDTH, HEIGHT)) {
		return false;
	}

	mesh meshCube;
	float fTheta = 0.0f;

	meshCube.tris = {
		// SOUTH
		{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		// EAST                                                      
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

		// NORTH                                                     
		{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

		// WEST                                                      
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

		// TOP                                                       
		{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

		// BOTTOM                                                    
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
	};

	mat4x4 matProj;

	float fNear = 0.1f;
	float fFar = 1000.0f;
	float fFov = 90.0f;
	float fAspectRatio = (float)HEIGHT / (float)WIDTH;
	float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * M_PI);


	matProj.m[0][0] = fAspectRatio * fFovRad;
	matProj.m[1][1] = fFovRad;
	matProj.m[2][2] = fFar / (fFar - fNear);
	matProj.m[2][3] = 1.0f;
	matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
	matProj.m[3][3] = 0.0f;

	while (1)
	{
		app.pollevent();

		app.clear();

		mat4x4 matRotZ, matRotX;

		fTheta += 1.0f * 0.03;
		
		// Rotation Z
		matRotZ.m[0][0] = cosf(fTheta);
		matRotZ.m[0][1] = sinf(fTheta);
		matRotZ.m[1][0] = -sinf(fTheta);
		matRotZ.m[1][1] = cosf(fTheta);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		// Rotation X
		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cosf(fTheta * 0.5f);
		matRotX.m[1][2] = sinf(fTheta * 0.5f);
		matRotX.m[2][1] = -sinf(fTheta * 0.5f);
		matRotX.m[2][2] = cosf(fTheta * 0.5f);
		matRotX.m[3][3] = 1;


		for (auto tri : meshCube.tris) {

			triangle triProjected, triTranslated, triRotatedZ, triRotatedX, triRotatedZX;

			multVec3Mat44(tri.p[0], triRotatedZ.p[0], matRotZ);
			multVec3Mat44(tri.p[1], triRotatedZ.p[1], matRotZ);
			multVec3Mat44(tri.p[2], triRotatedZ.p[2], matRotZ);

			multVec3Mat44(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
			multVec3Mat44(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
			multVec3Mat44(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);


			triTranslated = triRotatedZX;
			triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
			triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
			triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;


			multVec3Mat44(triTranslated.p[0], triProjected.p[0], matProj);
			multVec3Mat44(triTranslated.p[1], triProjected.p[1], matProj);
			multVec3Mat44(triTranslated.p[2], triProjected.p[2], matProj);

			// scale into view

			triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
			triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
			triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;

			triProjected.p[0].x *= 0.5f * (float)WIDTH;
			triProjected.p[0].y *= 0.5f * (float)HEIGHT;
			triProjected.p[1].x *= 0.5f * (float)WIDTH;
			triProjected.p[1].y *= 0.5f * (float)HEIGHT;
			triProjected.p[2].x *= 0.5f * (float)WIDTH;
			triProjected.p[2].y *= 0.5f * (float)HEIGHT;

			app.drawtriangle(
				triProjected.p[0].x, triProjected.p[0].y,
				triProjected.p[1].x, triProjected.p[1].y,
				triProjected.p[2].x, triProjected.p[2].y,
				0xffffffff);
		}

		app.update();

	}
}
