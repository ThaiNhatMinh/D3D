#pragma once

#include "..\base.h"


class Cube
{
private:
	ID3D10Device* m_pDevice;
	Mesh m_Mesh;
public:
	void Init(ID3D10Device* pDevice, float scale);
	void Draw();
};