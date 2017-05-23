#include "DirectInput.h"

DirectInput* gDInput;

DirectInput::DirectInput(BaseD3D * pApp, DWORD keyboardCoopFlags, DWORD mouseCoopFlag):hasInit(0)
{
	Init(pApp, keyboardCoopFlags, mouseCoopFlag);
}

DirectInput::DirectInput() :m_pDInput(0), m_pKeyboard(0), m_pMouse(0), hasInit(0)
{
}

DirectInput::~DirectInput()
{
	R(m_pDInput);
	m_pKeyboard->Unacquire();
	R(m_pKeyboard);
	m_pMouse->Unacquire();
	R(m_pMouse);

}

void DirectInput::Init(BaseD3D* pApp, DWORD keyboardCoopFlags, DWORD mouseCoopFlag)
{
	if (hasInit) return;

	SetActiveWindow(pApp->GetMainWnd());
	ZeroMemory(m_KeyState, sizeof(m_KeyState));
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));

	HR(DirectInput8Create(pApp->GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDInput, 0));

	HR(m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, 0));
	HR(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard));
	HR(m_pKeyboard->SetCooperativeLevel(pApp->GetMainWnd(), keyboardCoopFlags));
	HR(m_pKeyboard->Acquire());

	HR(m_pDInput->CreateDevice(GUID_SysMouse, &m_pMouse, 0));
	HR(m_pMouse->SetDataFormat(&c_dfDIMouse2));
	HR(m_pMouse->SetCooperativeLevel(pApp->GetMainWnd(), mouseCoopFlag));
	HR(m_pMouse->Acquire());

	hasInit = 1;
}

void DirectInput::Update()
{
	HRESULT hr = m_pKeyboard->GetDeviceState(sizeof(m_KeyState), (void**)&m_KeyState);
	if (FAILED(hr))
	{
		// Keyboard lost, zero out keyboard data structure.
		ZeroMemory(m_KeyState, sizeof(m_KeyState));

		// Try to acquire for next time we poll.
		hr = m_pKeyboard->Acquire();
	}

	hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&m_MouseState);
	if (FAILED(hr))
	{
		// Keyboard lost, zero out keyboard data structure.
		ZeroMemory(&m_MouseState, sizeof(m_MouseState));

		// Try to acquire for next time we poll.
		hr = m_pMouse->Acquire();
	}
}

bool DirectInput::KeyDown(char key)
{
	return (m_KeyState[key] & 0x80) != 0;
}

bool DirectInput::MouseButtonDown(int button)
{
	return  (m_MouseState.rgbButtons[button] & 0x80) != 0;;
}

float DirectInput::mouseDX()
{
	return (float)m_MouseState.lX;
}

float DirectInput::mouseDY()
{
	return (float)m_MouseState.lY;
}

float DirectInput::mouseDZ()
{
	return (float)m_MouseState.lZ;
}

DirectInput & DirectInput::operator=(const DirectInput & other)
{
	// TODO: insert return statement here
	return *this;
}
