#include"fbstdafx.h"
#include "Mouse.h"

HRESULT Mouse::Initialize(LPDIRECTINPUT8 Dinput)
{
	//�f�o�C�X�쐬
	if (FAILED(Dinput->CreateDevice(
		GUID_SysMouse,	// �}�E�X���󂯕t����
		&_MouseDevice,	// IDirectInputDevice8�|�C���^
		NULL)))			// �g��Ȃ�
	{
		return false;
	}

	//�󂯎��\���̂̃t�H�[�}�b�g��ݒ�
	if (FAILED(_MouseDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return false;
	}


	// ���̓f�o�C�X�ւ̃A�N�Z�X�����擾
	_MouseDevice->Acquire();

	HWND hWnd = FindWindow("DECIDE", NULL);
	g_MainWindow;
	_MouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	// �f�o�C�X�̐ݒ�    
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(DIPROPDWORD);
	diprop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	// ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j
	//���Βl�͑O�t���[������̑��Βl(�ړ��ʁH)
	//��Βl�͑��Βl�̗݌v
	diprop.dwData = DIPROPAXISMODE_REL;
	_MouseDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	// ���͐���J�n
	_MouseDevice->Acquire();
			
	return D3D_OK;
}

void Mouse::Update()
{
	//�O�t���[���̂�ɃR�s�[
	memcpy(&_Old, &_State, sizeof(DIMOUSESTATE2));
	// �l�̏�����    
	ZeroMemory(&_State, sizeof(DIMOUSESTATE2));
	// �l�̍X�V 
	if (FAILED(_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &_State)))
	{
		//���s�����̂ł�����x�󂯎���悤�ɂ���
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
	//��Βl���擾
	GetCursorPos(&pos);
	//���ΓI�Ȉʒu���擾
	ScreenToClient(hwnd, &pos);
	return Vector2(pos.x, pos.y);
}
