#include "XInput.h"

XInput::XInput()
{

}

void XInput::Initialize(int index)
{
	_UserIndex = index;
	ZeroMemory(&_State, sizeof(XINPUT_STATE)); // ������
	ZeroMemory(&_BeforeState, sizeof(XINPUT_STATE)); // ������
}

void XInput::Update()
{
	//�O�̃X�e�[�g�ۑ�
	_BeforeState = _State;
	//�X�e�[�g�擾
	DWORD er = XInputGetState(_UserIndex, &_State);
	//�f�b�h�]�[���Ȃ�l���ۂ߂�
	_Rounding();
	//�ڑ��ł��Ă��邩�ǂ���
	if (er == ERROR_DEVICE_NOT_CONNECTED)
	{
		_IsConnect = false;
	}
	else if (er == ERROR_SUCCESS)
	{
		_IsConnect = true;
	}
}

bool XInput::IsConnected()
{
	return _IsConnect;
}

bool XInput::IsPushButton(int gamepad)
{
	if (!(_BeforeState.Gamepad.wButtons & gamepad) &&		//�O�t���[���ɉ�����Ă��Ȃ�
		(_State.Gamepad.wButtons & gamepad))				//��������Ă���
	{
		return true;
	}
	return false;
}

bool XInput::IsPressButton(int gamepad)
{
	//�_���ς��Ƃ�
	return ((_State.Gamepad.wButtons & gamepad) > 0);
}

bool XInput::IsPushAnalog(AnalogE a)
{
	//�ڑ��m�F
	if (!IsConnected())
	{
		return false;
	}

	switch (a)
	{
	case L_TRIGGER:
		if (_BeforeState.Gamepad.bLeftTrigger == 0 &&		//������Ă��Ȃ�
			_State.Gamepad.bLeftTrigger > 0)				//��������Ă���
			return true;
		break;
	case R_TRIGGER:
		if (_BeforeState.Gamepad.bRightTrigger == 0 &&		//������Ă��Ȃ�
			_State.Gamepad.bRightTrigger > 0)				//��������Ă���
			return true;
		break;
	case L_STICKR:
		if (_BeforeState.Gamepad.sThumbLX == 0 &&		//������Ă��Ȃ�
			_State.Gamepad.sThumbLX > 0)				//��������Ă���
			return true;
		break;
	case L_STICKL:
		if (_BeforeState.Gamepad.sThumbLX == 0 &&		//������Ă��Ȃ�
			_State.Gamepad.sThumbLX < 0)				//��������Ă���
			return true;
		break;
	case L_STICKU:
		if (_BeforeState.Gamepad.sThumbLY == 0 &&		//������Ă��Ȃ�
			_State.Gamepad.sThumbLY > 0)				//��������Ă���
			return true;
		break;
	case L_STICKD:
		if (_BeforeState.Gamepad.sThumbLY == 0 &&		//������Ă��Ȃ�
			_State.Gamepad.sThumbLY < 0)				//��������Ă���
			return true;
		break;
	case R_STICKR:
		if (_BeforeState.Gamepad.sThumbRX == 0 &&		//������Ă��Ȃ�
			_State.Gamepad.sThumbRX > 0)				//��������Ă���
			return true;
		break;
	case R_STICKL:
		if (_BeforeState.Gamepad.sThumbRX == 0 &&		//������Ă��Ȃ�
			_State.Gamepad.sThumbRX < 0)				//��������Ă���
			return true;
		break;
	case R_STICKU:
		if (_BeforeState.Gamepad.sThumbRY == 0 &&		//������Ă��Ȃ�
			_State.Gamepad.sThumbRY > 0)				//��������Ă���
			return true;
		break;
	case R_STICKD:
		if (_BeforeState.Gamepad.sThumbRY == 0 &&		//������Ă��Ȃ�
			_State.Gamepad.sThumbRY < 0)				//��������Ă���
			return true;
		break;
	default:
		break;
	}

	return false;
}

Vector2 XInput::GetAnalog(AnalogInputE in)
{
	Vector2 value;
	switch (in)
	{
	case TRIGGER:
		value.x = _State.Gamepad.bRightTrigger;
		value.y = _State.Gamepad.bLeftTrigger;
		break;
	case L_STICK:
		value.x = _State.Gamepad.sThumbLX;
		value.y = _State.Gamepad.sThumbLY;
		break;
	case R_STICK:
		value.x = _State.Gamepad.sThumbRX;
		value.y = _State.Gamepad.sThumbRY;
		break;
	default:
		break;
	}
	return value;
}
;

void XInput::Vibration(int Rmoter, int Lmoter)
{
	//�U��
	XINPUT_VIBRATION vibration;
	//������
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wRightMotorSpeed = Rmoter; //�E���[�^�[�̉�]�� use any value between 0-65535 here
	vibration.wLeftMotorSpeed = Lmoter; //�����[�^�[�̉�]�� use any value between 0-65535 here
	//�U���Z�b�g , �G���[�`�F�b�N
	if(XInputSetState(_UserIndex, &vibration) == ERROR_DEVICE_NOT_CONNECTED)
	{
		//�ڑ�����ĂȂ�
		_IsConnect = false;
	}
}

void XInput::_Rounding()
{
	/*const int LowerLimit = 100;
	const int UpperLimit = 65535;*/

	if (-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE < _State.Gamepad.sThumbLY &&
		_State.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		_State.Gamepad.sThumbLY = 0;
	}

	if (-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE < _State.Gamepad.sThumbLX &&
		_State.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		_State.Gamepad.sThumbLX = 0;
	}

	if (-XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE < _State.Gamepad.sThumbRY &&
		_State.Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{ _State.Gamepad.sThumbRY = 0; }

	if (-XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE < _State.Gamepad.sThumbRX &&
		_State.Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{ _State.Gamepad.sThumbRX = 0; }
}