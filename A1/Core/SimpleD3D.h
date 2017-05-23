#pragma once
#include "..\base.h"


class SimpleD3D : public BaseD3D
{
private:

	Cube m_Cube;
	ID3D10Effect* m_pFX;
	ID3D10EffectTechnique* m_pTechFX;
	ID3D10InputLayout* m_pInputLayout;
	ID3D10EffectMatrixVariable* m_pfxWVP;

	D3DXMATRIX m_View;
	D3DXMATRIX m_Proj;
	D3DXMATRIX m_WVP;
	Camera m_Camera;
	float mTheta;
	float mPhi;
protected:
	void BuildFX();
	void BuildInputLayout();
public:
	SimpleD3D(HINSTANCE hInstance);
	~SimpleD3D();

	virtual void	OnInitApp();
	virtual void	OnResize();
	virtual void	OnUpdateScene(float dt);
	virtual void	OnDrawScene();
	//virtual LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
};