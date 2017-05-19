#pragma once
#include "..\base.h"


class SimpleD3D : public BaseD3D
{
public:
	SimpleD3D(HINSTANCE hInstance);
	~SimpleD3D();

	virtual void	OnInitApp();
	virtual void	OnResize();
	virtual void	OnUpdateScene(float dt);
	virtual void	OnDrawScene();
	//virtual LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
};