#include "..\base.h"

void SimpleD3D::BuildFX()
{
	DWORD flag = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined(_DEBUG) || defined(DEBUG)
	flag |= D3D10_SHADER_DEBUG;
	flag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
	ID3D10Blob* compilationErrors = 0;
	HRESULT hr = 0;
	hr = D3DX10CreateEffectFromFile(L"Color.fx", 0, 0, "fx_4_0", flag, 0, m_pD3DDevice, 0, 0, &m_pFX, &compilationErrors,0);

	if (FAILED(hr))
	{
		if (compilationErrors)
		{
			MessageBoxA(0, (char*)compilationErrors->GetBufferPointer(), 0, 0);
			R(compilationErrors);
		}
		DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX10CreateEffectFromFile", true);
	}

	m_pTechFX = m_pFX->GetTechniqueByName("ColorTech");
	m_pfxWVP = m_pFX->GetVariableByName("WVP")->AsMatrix();
}

void SimpleD3D::BuildInputLayout()
{
	D3D10_INPUT_ELEMENT_DESC ieDesc[] = 
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D10_INPUT_PER_VERTEX_DATA,0},
		{"COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D10_INPUT_PER_VERTEX_DATA,0}
	};

	D3D10_PASS_DESC passDesc;
	m_pTechFX->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_pD3DDevice->CreateInputLayout(ieDesc, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_pInputLayout));

}

SimpleD3D::SimpleD3D(HINSTANCE hInstance):BaseD3D(hInstance),m_pFX(0),m_pTechFX(0),m_pInputLayout(0),m_pfxWVP(0), mTheta(0),m_Camera(Vec3(0,0,5.0f),Vec3(0.0,0.0,0.0),Vec3(0,1.0f,0))
{
	D3DXMatrixIdentity(&m_View);
	D3DXMatrixIdentity(&m_Proj);
	D3DXMatrixIdentity(&m_WVP);
}

SimpleD3D::~SimpleD3D()
{
	if (m_pD3DDevice)
		m_pD3DDevice->ClearState();
}

void SimpleD3D::OnInitApp()
{
	BaseD3D::OnInitApp();

	BuildFX();
	BuildInputLayout();
	m_Cube.Init(m_pD3DDevice, 1.0f);

	
}

void SimpleD3D::OnResize()
{
	BaseD3D::OnResize();

	float aspect = (float)m_iClientWidth / m_iClientHeight;
	D3DXMatrixPerspectiveFovLH(&m_Proj, 0.25f*PI, aspect, 1.0f, 1000.0f);
}

void SimpleD3D::OnUpdateScene(float dt)
{
	BaseD3D::OnUpdateScene(dt);
	m_Camera.Update(dt);
	Vec3 pos = m_Camera.GetPosition();
	
	// Update angles based on input to orbit camera around box.
	//if (GetAsyncKeyState('A') & 0x8000)	mTheta -= 2.0f*dt;
	//if (GetAsyncKeyState('D') & 0x8000)	mTheta += 2.0f*dt;
	//if (GetAsyncKeyState('W') & 0x8000)	mPhi -= 2.0f*dt;
	//if (GetAsyncKeyState('S') & 0x8000)	mPhi += 2.0f*dt;
	
	/*if (gDInput->KeyDown(DIK_A))	mTheta -= 2.0f*dt;
	if (gDInput->KeyDown(DIK_D))	mTheta += 2.0f*dt;
	if (gDInput->KeyDown(DIK_W))	mPhi -= 2.0f*dt;
	if (gDInput->KeyDown(DIK_S))	mPhi += 2.0f*dt;
	// Restrict the angle mPhi.
	if (mPhi < 0.1f)	mPhi = 0.1f;
	if (mPhi > PI - 0.1f)	mPhi = PI - 0.1f;

	// Convert Spherical to Cartesian coordinates: mPhi measured from +y
	// and mTheta measured counterclockwise from -z.
	float x = 5.0f*sinf(mPhi)*sinf(mTheta);
	float z = -5.0f*sinf(mPhi)*cosf(mTheta);
	float y = 5.0f*cosf(mPhi);

	// Build the view matrix.
	D3DXVECTOR3 pos(x, y, z);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_View, &pos, &target, &up);
	*/
	m_View = m_Camera.GetViewMatrix();
}

void SimpleD3D::OnDrawScene()
{
	BaseD3D::OnDrawScene();
	// Restore default states, input layout and primitive topology 
	// because mFont->DrawText changes them.  Note that we can 
	// restore the default states by passing null.
	m_pD3DDevice->OMSetDepthStencilState(0, 0);
	float blendFactors[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_pD3DDevice->OMSetBlendState(0, blendFactors, 0xffffffff);
	m_pD3DDevice->IASetInputLayout(m_pInputLayout);

	// set constants
	m_WVP = m_View*m_Proj;
	m_pfxWVP->SetMatrix((float*)&m_WVP);

	D3D10_TECHNIQUE_DESC techDesc;
	m_pTechFX->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pTechFX->GetPassByIndex(p)->Apply(0);

		m_Cube.Draw();
	}

	// We specify DT_NOCLIP, so we do not care about width/height of the rect.
	RECT R = { 5, 5, 0, 0 };
	m_pFont->DrawText(0, m_FrameStatus.c_str(), -1, &R, DT_NOCLIP, BLACK);

	m_pSwapChain->Present(0, 0);
}