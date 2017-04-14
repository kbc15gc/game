#include "stdafx.h"
#include "Mouse.h"

HRESULT Mouse::Initialize(LPDIRECTINPUT8 Dinput)
{
	//�f�o�C�X�쐬
	if (FAILED(Dinput->CreateDevice(
		GUID_SysMouse,	// �}�E�X���󂯕t����
		&mouseDevice,	// IDirectInputDevice8�|�C���^
		NULL)))			// �g��Ȃ�
	{
		return false;
	}

	//�󂯎��\���̂̃t�H�[�}�b�g��ݒ�
	if (FAILED(mouseDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return false;
	}


	// ���̓f�o�C�X�ւ̃A�N�Z�X�����擾
	mouseDevice->Acquire();

	HWND hWnd = FindWindow("DECIDE", NULL);
	g_MainWindow;
	mouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

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
	mouseDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	// ���͐���J�n
	mouseDevice->Acquire();
			
	return D3D_OK;
}

void Mouse::Update()
{
	// �l�̏�����    
	ZeroMemory(&mouseState, sizeof(mouseState));
	// �l�̍X�V 
	if (FAILED(mouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState)))
	{
		//���s�����̂ł�����x�󂯎���悤�ɂ���
		mouseDevice->Acquire();
		mouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState);
	}
}

int Mouse::GetValue(MouseInE m)
{
	int value = false;
	switch (m)
	{
	case L_CLICK:
		value = (mouseState.rgbButtons[0] & 0x80);
		break;
	case R_CLICK:
		value = (mouseState.rgbButtons[1] & 0x80);
		break;
	case WHEEL_PUSH:
		value = (mouseState.rgbButtons[2] & 0x80);
		break;
	case X:
		value = mouseState.lX;
		break;
	case Y:
		value = mouseState.lY;
		break;
	case WHEEL_ROLL:
		value = mouseState.lZ;
		break;
	default:
		break;
	}
	return value;
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
