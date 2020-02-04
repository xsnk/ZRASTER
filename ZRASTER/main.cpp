#include "zraster.h"
#include "geometry.h"
#include "color.h"

#include <algorithm>

int main()
{
	ZRASTER app;
	if (!app.init("ZRASTER", WIDTH, HEIGHT)) {
		return false;
	}

	mesh meshCube;
	mat4x4 matProj;
	vec3d vCamera = {};

	float fTheta = 0.0f;

//	meshCube.tris = {
//		// SOUTH
//		{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
//		{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },
//
//		// EAST                                                      
//		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
//		{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },
//
//		// NORTH                                                     
//		{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
//		{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },
//
//		// WEST                                                      
//		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
//		{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },
//
//		// TOP                                                       
//		{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
//		{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },
//
//		// BOTTOM                                                    
//		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
//		{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
//	};

	if (!meshCube.loadfromobj("cow.obj")) {
		printf("Cant find file");
		exit(EXIT_FAILURE);
	}

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


		std::vector<triangle> vecTrianglesToRaster;

		for (auto tri : meshCube.tris) {

			triangle triProjected, triTranslated, triRotatedZ, triRotatedX, triRotatedZX;

			multVec3Mat44(tri.p[0], triRotatedZ.p[0], matRotZ);
			multVec3Mat44(tri.p[1], triRotatedZ.p[1], matRotZ);
			multVec3Mat44(tri.p[2], triRotatedZ.p[2], matRotZ);

			multVec3Mat44(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
			multVec3Mat44(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
			multVec3Mat44(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

			// offset slightly

			triTranslated = triRotatedZX;
			triTranslated.p[0].z = triRotatedZX.p[0].z + 12.0f;
			triTranslated.p[1].z = triRotatedZX.p[1].z + 12.0f;
			triTranslated.p[2].z = triRotatedZX.p[2].z + 12.0f;


			vec3d normal, line1, line2;
			line1.x = triTranslated.p[1].x - triTranslated.p[0].x;
			line1.y = triTranslated.p[1].y - triTranslated.p[0].y;
			line1.z = triTranslated.p[1].z - triTranslated.p[0].z;

			line2.x = triTranslated.p[2].x - triTranslated.p[0].x;
			line2.y = triTranslated.p[2].y - triTranslated.p[0].y;
			line2.z = triTranslated.p[2].z - triTranslated.p[0].z;

			normal.x = line1.y * line2.z - line1.z * line2.y;
			normal.y = line1.z * line2.x - line1.x * line2.z;
			normal.z = line1.x * line2.y - line1.y * line2.x;

			float l = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
			normal.x /= l;
			normal.y /= l;
			normal.z /= l;

			// TODO(ZED): make Cross product, lenght of the vector and vector normalisation functions
			//            and stop coding like a HOBO

			if (normal.x * (triTranslated.p[0].x - vCamera.x) +
				normal.y * (triTranslated.p[0].y - vCamera.y) +
				normal.z * (triTranslated.p[0].z - vCamera.z) < 0) {

				vec3d light_direction = { 0.0f, 0.0f, -1.0f };
				float l = sqrtf(light_direction.x * light_direction.x +
					light_direction.y * light_direction.y +
					light_direction.z * light_direction.z);

				light_direction.x /= l;
				light_direction.y /= l;
				light_direction.z /= l;

				float dp = normal.x * light_direction.x + normal.y * light_direction.y + normal.z * light_direction.z;

				uint32_t c = getcolor(dp);
				triTranslated.color = c;

				multVec3Mat44(triTranslated.p[0], triProjected.p[0], matProj);
				multVec3Mat44(triTranslated.p[1], triProjected.p[1], matProj);
				multVec3Mat44(triTranslated.p[2], triProjected.p[2], matProj);

				// scale into view

				triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
				triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
				triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;
				triProjected.color = c;

				triProjected.p[0].x *= 0.5f * (float)WIDTH;
				triProjected.p[0].y *= 0.5f * (float)HEIGHT;
				triProjected.p[1].x *= 0.5f * (float)WIDTH;
				triProjected.p[1].y *= 0.5f * (float)HEIGHT;
				triProjected.p[2].x *= 0.5f * (float)WIDTH;
				triProjected.p[2].y *= 0.5f * (float)HEIGHT;

				vecTrianglesToRaster.push_back(triProjected);

			}
		}


		std::sort(vecTrianglesToRaster.begin(), vecTrianglesToRaster.end(), [](triangle& t1, triangle& t2)
		{
			float z1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
			float z2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
			return z1 > z2;

		});


		for (auto& triProjected : vecTrianglesToRaster) {

			app.filltriangle(
				triProjected.p[0].x, triProjected.p[0].y,
				triProjected.p[1].x, triProjected.p[1].y,
				triProjected.p[2].x, triProjected.p[2].y,
				triProjected.color);

			// Debug only
			//app.drawtriangle(
			//	triProjected.p[0].x, triProjected.p[0].y,
			//	triProjected.p[1].x, triProjected.p[1].y,
			//	triProjected.p[2].x, triProjected.p[2].y,
			//	0xffffffff);
		}

		app.update();

	}
}
