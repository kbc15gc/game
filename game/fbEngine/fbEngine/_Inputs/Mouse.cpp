#include"fbstdafx.h"
#include "Mouse.h"

HRESULT Mouse::Initialize(LPDIRECTINPUT8 Dinput)
{
	//デバイス作成
	if (FAILED(Dinput->CreateDevice(
		GUID_SysMouse,	// マウスを受け付ける
		&_MouseDevice,	// IDirectInputDevice8ポインタ
		NULL)))			// 使わない
	{
		return false;
	}

	//受け取る構造体のフォーマットを設定
	if (FAILED(_MouseDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return false;
	}


	// 入力デバイスへのアクセス権を取得
	_MouseDevice->Acquire();

	HWND hWnd = FindWindow("DECIDE", NULL);
	g_MainWindow;
	_MouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	// デバイスの設定    
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(DIPROPDWORD);
	diprop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）
	//相対値は前フレームからの相対値(移動量？)
	//絶対値は相対値の累計
	diprop.dwData = DIPROPAXISMODE_REL;
	_MouseDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	// 入力制御開始
	_MouseDevice->Acquire();
			
	return D3D_OK;
}

void Mouse::Update()
{
	//前フレームのやつにコピー
	memcpy(&_Old, &_State, sizeof(DIMOUSESTATE2));
	// 値の初期化    
	ZeroMemory(&_State, sizeof(DIMOUSESTATE2));
	// 値の更新 
	if (FAILED(_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &_State)))
	{
		//失敗したのでもう一度受け取れるようにする
		_MouseDevice->Acquire();
		_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &_State);
	}
}

int Mouse::GetValue(MouseInE m)
{
	int value = false;
	switch (m)
	{
	case L_CLICK:
		value = (_State.rgbButtons[0] & 0x80);
		break;
	case R_CLICK:
		value = (_State.rgbButtons[1] & 0x80);
		break;
	case WHEEL_PUSH:
		value = (_State.rgbButtons[2] & 0x80);
		break;
	case X:
		value = _State.lX;
		break;
	case Y:
		value = _State.lY;
		break;
	case WHEEL_ROLL:
		value = _State.lZ;
		break;
	default:
		break;
	}
	return value;
}

int Mouse::GetUp(MouseInE m)
{
	bool click = false;
	switch (m)
	{
	case L_CLICK:
		click = ((_Old.rgbButtons[0] & 0x80) > 0) && ((_State.rgbButtons[0] & 0x80) == 0);
		break;
	case R_CLICK:
		click = ((_Old.rgbButtons[1] & 0x80) > 0) && ((_State.rgbButtons[1] & 0x80) == 0);
		break;
	case WHEEL_PUSH:
		click = ((_Old.rgbButtons[2] & 0x80) > 0) && ((_State.rgbButtons[2] & 0x80) == 0);
		break;
	case X:
		click = (_Old.lX != 0.0f) && (_State.lX == 0.0f);
		break;
	case Y:
		click = (_Old.lY != 0.0f) && (_State.lY == 0.0f);
		break;
	case WHEEL_ROLL:
		click = (_Old.lZ != 0.0f) && (_State.lZ == 0.0f);
		break;
	default:
		break;
	}
	return click;
}

int Mouse::GetDown(MouseInE m)
{
	bool click = false;
	switch (m)
	{
	case L_CLICK:
		click = ((_Old.rgbButtons[0] & 0x80) == 0) && ((_State.rgbButtons[0] & 0x80) > 0);
		break;
	case R_CLICK:
		click = ((_Old.rgbButtons[1] & 0x80) == 0) && ((_State.rgbButtons[1] & 0x80) > 0);
		break;
	case WHEEL_PUSH:
		click = ((_Old.rgbButtons[2] & 0x80) == 0) && ((_State.rgbButtons[2] & 0x80) > 0);
		break;
	case X:
		click = (_Old.lX == 0.0f) && (_State.lX != 0.0f);
		break;
	case Y:
		click = (_Old.lY == 0.0f) && (_State.lY != 0.0f);
		break;
	case WHEEL_ROLL:
		click = (_Old.lZ == 0.0f) && (_State.lZ != 0.0f);
		break;
	default:
		break;
	}
	return click;
}

Vector2 Mouse::GetCursorPosOnScreen()
{
	POINT pos;
	GetCursorPos(&pos);
	return Vector2(pos.x, pos.y);
}

Vector2 Mouse::GetCursorPosOnWindow(HWND hwnd)
{
	POINT pos;
	//絶対値を取得
	GetCursorPos(&pos);
	//相対的な位置を取得
	ScreenToClient(hwnd, &pos);
	return Vector2(pos.x, pos.y);
}
