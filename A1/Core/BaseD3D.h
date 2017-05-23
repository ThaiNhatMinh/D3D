#pragma once
#include "..\base.h"


class BaseD3D
{
public:
	BaseD3D(HINSTANCE hInstance);
	virtual ~BaseD3D();

	HINSTANCE		GetInstance();
	HWND			GetMainWnd();
	int				Run();

	virtual void	OnInitApp();
	virtual void	OnResize();
	virtual void	OnUpdateScene(float dt);
	virtual void	OnDrawScene();
	virtual LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);
protected:

	void			InitWindows();
	void			InitD3D();

protected:
	HINSTANCE			m_hInstance;
	HWND				m_Hwnd;
	bool				m_bAppPaused;
	bool				m_bMinimized;
	bool				m_bMaximized;
	bool				m_bResizing;

	std::wstring		m_FrameStatus;

	GameTimer			m_Timer;
	ID3D10Device*		m_pD3DDevice;
	IDXGISwapChain*		m_pSwapChain;
	ID3D10Texture2D*	m_pDepthStencilBuffer;
	ID3D10RenderTargetView* m_pRenderTargetView;
	ID3D10DepthStencilView* m_pDepthStencilView;
	ID3DX10Font*		m_pFont;

	std::wstring		m_WndCaption;
	D3D10_DRIVER_TYPE	m_D3DDriveType;
	D3DXCOLOR			m_ClearColor;
	int					m_iClientWidth;
	int					m_iClientHeight;
	int					m_iScreenWidth;
	int					m_iScreenHeight;
	int					m_iBoderSize[2];
};