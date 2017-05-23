#include "..\base.h"


bool Plane::Inside(const Vec3 &point) const
{
	// Inside the plane is defined as the direction the normal is facing
	float result = D3DXPlaneDotCoord(this, &point);
	return (result >= 0.0f);
}

bool Plane::Inside(const Vec3 &point, const float radius) const
{
	float fDistance;	// calculate our distances to each of the planes

						// find the distance to this plane
	fDistance = D3DXPlaneDotCoord(this, &point);

	// if this distance is < -radius, we are outside
	return (fDistance >= -radius);
}

//
// Frustum::Frustum					- Chapter 14, page 477
//
Frustum::Frustum()
{
	m_Fov = PI / 4.0f;		// default field of view is 90 degrees
	m_Aspect = 1.0f;			// default aspect ratio is 1:1
	m_Near = 1.0f;				// default near clip plane is 1m away from the camera
	m_Far = 1000.0f;				// default near clip plane is 100m away from the camera
}


//
// Frustum::Inside					- Chapter 14, page 477
//
bool Frustum::Inside(const Vec3 &point) const
{
	for (int i = 0; i<NumPlanes; ++i)
	{
		if (!m_Planes[i].Inside(point))
			return false;
	}

	return true;
}


//
// Frustum::Inside					- Chapter 14, page 477
//
bool Frustum::Inside(const Vec3 &point, const float radius) const
{
	for (int i = 0; i < NumPlanes; ++i)
	{
		if (!m_Planes[i].Inside(point, radius))
			return false;
	}

	// otherwise we are fully in view
	return(true);
}

//
// Frustum::Init					- Chapter 14, page 477
//
void Frustum::Init(const float fov, const float aspect, const float nearClip, const float farClip)
{
	m_Fov = fov;
	m_Aspect = aspect;
	m_Near = nearClip;
	m_Far = farClip;

	float tanFovOver2 = (float)tan(m_Fov / 2.0f);
	Vec3 nearRight = (m_Near * tanFovOver2) * m_Aspect * g_Right;
	Vec3 farRight = (m_Far * tanFovOver2) * m_Aspect * g_Right;
	Vec3 nearUp = (m_Near * tanFovOver2) * g_Up;
	Vec3 farUp = (m_Far * tanFovOver2)  * g_Up;

	// points start in the upper right and go around clockwise
	m_NearClip[0] = (m_Near * g_Forward) - nearRight + nearUp;
	m_NearClip[1] = (m_Near * g_Forward) + nearRight + nearUp;
	m_NearClip[2] = (m_Near * g_Forward) + nearRight - nearUp;
	m_NearClip[3] = (m_Near * g_Forward) - nearRight - nearUp;

	m_FarClip[0] = (m_Far * g_Forward) - farRight + farUp;
	m_FarClip[1] = (m_Far * g_Forward) + farRight + farUp;
	m_FarClip[2] = (m_Far * g_Forward) + farRight - farUp;
	m_FarClip[3] = (m_Far * g_Forward) - farRight - farUp;

	// now we have all eight points. Time to construct 6 planes.
	// the normals point away from you if you use counter clockwise verts.

	Vec3 origin(0.0f, 0.0f, 0.0f);
	m_Planes[Near].Init(m_NearClip[2], m_NearClip[1], m_NearClip[0]);
	m_Planes[Far].Init(m_FarClip[0], m_FarClip[1], m_FarClip[2]);
	m_Planes[Right].Init(m_FarClip[2], m_FarClip[1], origin);
	m_Planes[Top].Init(m_FarClip[1], m_FarClip[0], origin);
	m_Planes[Left].Init(m_FarClip[0], m_FarClip[3], origin);
	m_Planes[Bottom].Init(m_FarClip[3], m_FarClip[2], origin);
}
Vec3 g_Right(1.0f, 0.0f, 0.0f);
Vec3 g_Up(0.0f, 1.0f, 0.0f);
Vec3 g_Forward(0.0f, 0.0f, 1.0f);


//
// Frustum::Render					- not described in the 4th edition
//

