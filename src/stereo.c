#include <math.h>

#include <GL/glut.h>
#include "stereo.h"

static float vfov = 0.0;
static float aspect = 1.0;
static float neara = 0.5;
static float fara = 500.0;

static float eye_sep = 1.0 / 30.0;
static float focus_dist = 1.0;

void stereo_proj_param(float _vfov, float _aspect, float _near, float _far)
{
	vfov = _vfov;
	aspect = _aspect;
	neara = _near;
	fara = _far;
}

void stereo_focus_dist(float d)
{
	focus_dist = d;
	eye_sep = d / 30.0;
}

void stereo_view_matrix(int eye)
{
	static const float offs_sign[] = {0.0f, 0.5f, -0.5f};
	glTranslatef(eye_sep * offs_sign[eye], 0, 0);
}

void stereo_proj_matrix(int eye)
{
	float vfov_rad = M_PI * vfov / 180.0;
	float top = neara * tan(vfov_rad * 0.5);
	float right = top * aspect;

	static const float offs_sign[] = {0.0f, 1.0, -1.0};	/* center, left, right */
	float frust_shift = offs_sign[eye] * (eye_sep * 0.5 * neara / focus_dist);

	glFrustum(-right + frust_shift, right + frust_shift, -top, top, neara, fara);
}
