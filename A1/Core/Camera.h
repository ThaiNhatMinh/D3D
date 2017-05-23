#pragma once
#include "..\base.h"


class Camera
{
private:
	Mat4x4 m_View;
	Vec3 m_Position;
	Vec3 m_Front;
	Vec3 m_Right;
	Vec3 m_Up;
	Vec3 WorldUp;
	float m_Speed;
	float m_Pitch;	// x axis
	float m_Yaw;	// y axis
	float MouseSensitivity;
protected:
	void UpdateVector();
public:
	Camera();
	
	// target and up vector must be normalize
	Camera(const Vec3& pos, const Vec3& target, const Vec3& up);
	~Camera() {};

	void Update(float dt);
	
	inline Mat4x4 GetViewMatrix() {	return m_View;	};

	inline void SetSpeed(float speed) { m_Speed = speed; };
	inline Vec3 GetFront() { return m_Front; };
	inline const Vec3 GetPosition()const { return m_Position; };
	inline Vec3& GetPosition() { return m_Position; };
	inline void SetPosition(Vec3 v) { m_Position = v; }
	inline Vec2 GetRorate() { return Vec2(m_Pitch, m_Yaw); };
	inline Vec3 GetUP() { return m_Up; };
};