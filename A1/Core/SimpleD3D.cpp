#include "..\base.h"

SimpleD3D::SimpleD3D(HINSTANCE hInstance):BaseD3D(hInstance)
{
}

SimpleD3D::~SimpleD3D()
{
	if (m_pD3DDevice)
		m_pD3DDevice->ClearState();
}

void SimpleD3D::OnInitApp()
{
	BaseD3D::OnInitApp();
}

void SimpleD3D::OnResize()
{
	BaseD3D::OnResize();
}

void SimpleD3D::OnUpdateScene(float dt)
{
	BaseD3D::OnUpdateScene(dt);
}

void SimpleD3D::OnDrawScene()
{
	BaseD3D::OnDrawScene();

	// We specify DT_NOCLIP, so we do not care about width/height of the rect.
	RECT R = { 5, 5, 0, 0 };
	m_pFont->DrawText(0, m_FrameStatus.c_str(), -1, &R, DT_NOCLIP, BLACK);

	m_pSwapChain->Present(0, 0);
}