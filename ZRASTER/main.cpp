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

	if (!meshCube.loadfromobj("cow.obj")) {
		printf("Cant find file");
		exit(EXIT_FAILURE);
	}

	float fNear = 0.1f;
	float fFar = 1000.0f;
	float fFov = 90.0f;
	float fAspectRatio = (float)HEIGHT / (float)WIDTH;

	matProj = mnewprojectionmatrix(fFov, fAspectRatio, fNear, fFar);

	while (1)
	{
		app.pollevent();

		app.clear();

		mat4x4 matRotZ, matRotX;
		fTheta += 1.0f * 0.03;
		
		matRotZ = mnewrotationmatrix_z(fTheta);
		matRotX = mnewrotationmatrix_x(fTheta);

		mat4x4 matTrans;
		matTrans = mnewtranslationmatrix(0.0f, 0.0f, 12.0f);

		mat4x4 matWorld;
		matWorld = mnewidentitymatrix();
		matWorld = mmulmat44mat44(matRotZ, matRotX);
		matWorld = mmulmat44mat44(matWorld, matTrans);


		std::vector<triangle> vecTrianglesToRaster;
		triangle triProjected, triTransformed;

		for (auto tri : meshCube.tris) {

			triTransformed.p[0] = mmulvec3mat44(matWorld, tri.p[0]);
			triTransformed.p[1] = mmulvec3mat44(matWorld, tri.p[1]);
			triTransformed.p[2] = mmulvec3mat44(matWorld, tri.p[2]);

			vec3d normal, line1, line2;

			line1 = vsubvector(triTransformed.p[1], triTransformed.p[0]);
			line2 = vsubvector(triTransformed.p[2], triTransformed.p[0]);

			normal = vcrossproduct(line1, line2);
			normal = vnormalise(normal);

			vec3d vCameraRay = vsubvector(triTransformed.p[0], vCamera);

			if (vdotproduct(normal, vCameraRay) < 0.0f) {

				vec3d light_direction = { 0.0f, 1.0f, -1.0f };
				light_direction = vnormalise(light_direction);

				float dp = std::max(0.1f, vdotproduct(light_direction, normal));

				uint32_t c = getcolor(dp);
				triTransformed.color = c;

				triProjected.p[0] = mmulvec3mat44(matProj, triTransformed.p[0]);
				triProjected.p[1] = mmulvec3mat44(matProj, triTransformed.p[1]);
				triProjected.p[2] = mmulvec3mat44(matProj, triTransformed.p[2]);
				triProjected.color = triTransformed.color;

				triProjected.p[0] = sdivvector(triProjected.p[0], triProjected.p[0].w);
				triProjected.p[1] = sdivvector(triProjected.p[1], triProjected.p[1].w);
				triProjected.p[2] = sdivvector(triProjected.p[2], triProjected.p[2].w);


				// scale into view

				vec3d vOffsetView = { 1.0f, 1.0f, 0.0f };
				triProjected.p[0] = vaddvector(triProjected.p[0], vOffsetView);
				triProjected.p[1] = vaddvector(triProjected.p[1], vOffsetView);
				triProjected.p[2] = vaddvector(triProjected.p[2], vOffsetView);


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


		for (auto& tri : vecTrianglesToRaster) {

			app.filltriangle(
				tri.p[0].x, tri.p[0].y,
				tri.p[1].x, tri.p[1].y,
				tri.p[2].x, tri.p[2].y,
				tri.color);

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
