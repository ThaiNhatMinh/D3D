#include "..\base.h"
#include "Camera.h"

void Camera::UpdateVector()
{
	Vec3 front;
	front.x = -cos(D3DXToRadian(m_Yaw)) * cos(D3DXToRadian(m_Pitch));
	front.y = -sin(D3DXToRadian(m_Pitch));
	front.z = sin(D3DXToRadian(m_Yaw)) * cos(D3DXToRadian(m_Pitch));



	m_Front = Normalize(front);
	m_Right = Normalize(Cross( WorldUp, m_Front));
	m_Up = Normalize(Cross(m_Right, m_Front));
	D3DXMatrixLookAtLH(&m_View, &m_Position, &(m_Position+m_Front), &WorldUp);
}

Camera::Camera() :m_Position(0.0, 0.0, 0.0), m_Speed(50.0f), m_Pitch(0), m_Yaw(-90.0f), MouseSensitivity(0.25f)
{
	UpdateVector();
}

Camera::Camera(const Vec3 & pos, const Vec3 & target, const Vec3 & up)
{
	m_Position = pos;
	m_Front = Normalize(target - m_Position);
	WorldUp = up;
	m_Speed = 25.0f;
	m_Pitch = 0;
	m_Yaw = -90;
	MouseSensitivity = 0.1;
	UpdateVector();
}

void Camera::Update(float deltaTIme)
{
	if (gDInput->KeyDown(DIK_W)) m_Position += m_Front*m_Speed*deltaTIme;
	if (gDInput->KeyDown(DIK_S)) m_Position -= m_Front*m_Speed*deltaTIme;
	if (gDInput->KeyDown(DIK_D)) m_Position += m_Right*m_Speed*deltaTIme;
	if (gDInput->KeyDown(DIK_A)) m_Position -= m_Right*m_Speed*deltaTIme;

	m_Pitch += gDInput->mouseDY()*MouseSensitivity;
	m_Yaw += gDInput->mouseDX()*MouseSensitivity;

	//wchar_t debug[512];
	//swprintf(debug, L"POS: %f %f %f %f\n", gDInput->mouseDX(), gDInput->mouseDY(), m_Pitch, m_Yaw);
	//OutputDebugString(debug);

	if (m_Pitch > 89.0f) m_Pitch = 89.0f;
	if (m_Pitch < -89.0f) m_Pitch = -89.0f;

	UpdateVector();
}
