#include "..\base.h"


int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;
int (WINAPIV * __vsnwprintf)(wchar_t *, size_t, const wchar_t*, va_list) = _vsnwprintf;

BaseD3D::BaseD3D(HINSTANCE hInstance):m_hInstance(hInstance),m_pD3DDevice(0),m_pDepthStencilBuffer(0),m_pDepthStencilView(0),m_pRenderTargetView(0),m_pSwapChain(0)
{
	m_Hwnd = 0;
	m_bAppPaused = false;
	m_bMinimized = false;
	m_bMaximized = false;
	m_bResizing = false;

	m_FrameStatus = L"";

	
	m_pFont = 0;

	m_WndCaption = L"D3D10 Application";
	m_D3DDriveType = D3D10_DRIVER_TYPE_HARDWARE;
	m_ClearColor = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	m_iClientWidth = 800;
	m_iClientHeight = 600;
}

BaseD3D::~BaseD3D()
{
}

HINSTANCE BaseD3D::GetInstance()
{
	return m_hInstance;
}

HWND BaseD3D::GetMainWnd()
{
	return m_Hwnd;
}

int BaseD3D::Run()
{
	MSG msg = { 0 };
	m_Timer.Reset();
	while (msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Otherwise, do animation/game stuff.
		else
		{
			m_Timer.Tick();

			if (!m_bAppPaused)
				OnUpdateScene(m_Timer.GetDeltaTime());
			else
				Sleep(50);

			OnDrawScene();
		}
	}
	return (int)msg.wParam;
}

void BaseD3D::OnInitApp()
{
	InitWindows();
	InitD3D();
	m_Timer.Init();

	D3DX10_FONT_DESC fontDesc;
	fontDesc.Height = 24;
	fontDesc.Width = 0;
	fontDesc.Weight = 0;
	fontDesc.MipLevels = 1;
	fontDesc.Italic = false;
	fontDesc.CharSet = DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDesc.Quality = ANTIALIASED_QUALITY;
	fontDesc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	wcscpy(fontDesc.FaceName, L"Tahoma");

	D3DX10CreateFontIndirect(m_pD3DDevice, &fontDesc, &m_pFont);

}

void BaseD3D::OnResize()
{
	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.
	R(m_pDepthStencilBuffer);
	R(m_pDepthStencilView);
	R(m_pRenderTargetView);

	// Resize the swap chain and recreate the render target view.
	HR(m_pSwapChain->ResizeBuffers(1, m_iClientWidth, m_iClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	ID3D10Texture2D* backbuffer;
	HR(m_pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&backbuffer)));
	HR(m_pD3DDevice->CreateRenderTargetView(backbuffer, 0, &m_pRenderTargetView));

	R(backbuffer);

	// Create the depth/stencil buffer and view.
	D3D10_TEXTURE2D_DESC desc;
	desc.Width = m_iClientWidth;
	desc.Height = m_iClientHeight;
	desc.ArraySize = 1;
	desc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.MipLevels = 1;
	desc.MiscFlags = 0;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D10_USAGE_DEFAULT;
	
	HR(m_pD3DDevice->CreateTexture2D(&desc, 0, &m_pDepthStencilBuffer));
	HR(m_pD3DDevice->CreateDepthStencilView(m_pDepthStencilBuffer, 0, &m_pDepthStencilView));

	// Bind the render target view and depth/stencil view to the pipeline.
	m_pD3DDevice->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);


	//set view port
	D3D10_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = m_iClientWidth;
	vp.Height = m_iClientHeight;
	vp.MinDepth = 0;
	vp.MaxDepth = 1.0f;

	m_pD3DDevice->RSSetViewports(1, &vp);

}

void BaseD3D::OnUpdateScene(float dt)
{
	int fps = m_Timer.GetFPS(); // fps = frameCnt / 1
	float mspf = 1000.0f /(float) fps;

	std::wostringstream outs;
	outs.precision(6);
	outs << L"FPS: " << fps << L"\n"
		<< "Milliseconds: Per Frame: " << mspf;
	m_FrameStatus = outs.str();
}

void BaseD3D::OnDrawScene()
{
	m_pD3DDevice->ClearDepthStencilView(m_pDepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);
	m_pD3DDevice->ClearRenderTargetView(m_pRenderTargetView, m_ClearColor);
}


LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static BaseD3D* app = 0;

	switch (msg)
	{
		case WM_CREATE:
		{
			// Get the 'this' pointer we passed to CreateWindow via the lpParam parameter.
			CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
			app = (BaseD3D*)cs->lpCreateParams;
			return 0;
		}
	}

	// Don't start processing messages until after WM_CREATE.
	if (app)
		return app->MsgProc(msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT BaseD3D::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// WM_ACTIVATE is sent when the window is activated or deactivated.  
		// We pause the game when the window is deactivated and unpause it 
		// when it becomes active.  
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			m_bAppPaused = true;
			m_Timer.Stop();
		}
		else
		{
			m_bAppPaused = false;
			m_Timer.Start();
		}
		return 0;

		// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		// Save the new client area dimensions.
		m_iClientWidth = LOWORD(lParam);
		m_iClientHeight = HIWORD(lParam);
		if (m_pD3DDevice)
		{
			if (wParam == SIZE_MINIMIZED)
			{
				m_bAppPaused = true;
				m_bMinimized = true;
				m_bMaximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				m_bAppPaused = false;
				m_bMinimized = false;
				m_bMaximized = true;
				OnResize();
			}
			else if (wParam == SIZE_RESTORED)
			{

				// Restoring from minimized state?
				if (m_bMinimized)
				{
					m_bAppPaused = false;
					m_bMinimized = false;
					OnResize();
				}

				// Restoring from maximized state?
				else if (m_bMaximized)
				{
					m_bAppPaused = false;
					m_bMaximized = false;
					OnResize();
				}
				else if (m_bResizing)
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
				{
					OnResize();
				}
			}
		}
		return 0;

		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		m_bAppPaused = true;
		m_bResizing = true;
		m_Timer.Stop();
		return 0;

		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		m_bAppPaused = false;
		m_bResizing = false;
		m_Timer.Start();
		OnResize();
		return 0;

		// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// The WM_MENUCHAR message is sent when a menu is active and the user presses 
		// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
		// Don't beep when we alt-enter.
		return MAKELRESULT(0, MNC_CLOSE);

		// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;
	}

	return DefWindowProc(m_Hwnd, msg, wParam, lParam);
}

void BaseD3D::InitWindows()
{
	WNDCLASS wc;
	wc.style		= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc	= MainWndProc;
	wc.cbClsExtra	= 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"D3DWndClass";
	
	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"Register Class Failed", 0, 0);
		PostQuitMessage(0);
	}

	RECT R = { 0, 0, m_iClientWidth, m_iClientHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	m_Hwnd = CreateWindow(L"D3DWndClass", m_WndCaption.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, m_hInstance, this);

	if (!m_Hwnd)
	{
		MessageBox(0, L"Create Windows Failed", 0, 0);
		PostQuitMessage(0);
	}

	ShowWindow(m_Hwnd, SW_SHOW);
	UpdateWindow(m_Hwnd);

}

void BaseD3D::InitD3D()
{
	DXGI_SWAP_CHAIN_DESC sc;
	sc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sc.BufferDesc.Height = m_iClientHeight;
	sc.BufferDesc.Width = m_iClientWidth;
	sc.BufferDesc.RefreshRate.Denominator = 1;
	sc.BufferDesc.RefreshRate.Numerator = 60;
	sc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	// no multisampling
	sc.SampleDesc.Count = 1;
	sc.SampleDesc.Quality = 0;

	sc.BufferCount = 1;
	sc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sc.Flags = 0;
	sc.OutputWindow = m_Hwnd;
	sc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sc.Windowed = true;

	UINT flag = 0;
#if defined(_DEBUG)
	flag |= D3D10_CREATE_DEVICE_DEBUG;
#endif

	HR(D3DX10CreateDeviceAndSwapChain(0, D3D10_DRIVER_TYPE_HARDWARE, 0, flag, &sc, &m_pSwapChain, &m_pD3DDevice));

	OnResize();
}
